#include "mitXML.h"

MITXMLElement::MITXMLElement(std::string tagName): tagName(tagName), parent(nullptr) {
}

std::string MITXMLElement::getAttribute(std::string attrName) const {
	return this->attributes.at(attrName);
}

std::string MITXMLElement::getAttribute(std::string attrName, int *err) {
	if(!this->attributes.count(attrName)) {
		*err = E_NULL;
		return "";
	} else {
		*err = E_OK;
		return this->attributes.at(attrName);
	}
}

MITXMLNodeList MITXMLElement::selectNodes(std::string nodeName) const {
	MITXMLNodeList nodelist;
	for(auto &i: this->children) {
		if(i->tagName == nodeName) nodelist.item.emplace_back(i);
	}

	return nodelist;
}

size_t MITXMLNodeList::length() const {
	return this->item.size();
}

std::ifstream ifs;
MITXMLDocument::MITXMLDocument(const char *fileName) {
	this->load(fileName);
}

MITXMLDocument::~MITXMLDocument() {
	delete this->root;
}

bool MITXMLDocument::load(const char *fileName) {
	FILE *fp;
	fopen_s(&fp, fileName, "r");
	std::fseek(fp, 0, SEEK_END);
	size_t length = ftell(fp);
	std::fseek(fp, 0, SEEK_SET);

	char *buf = (char *)calloc(1, length + 2);
	std::fread(buf, length, 1, fp);

	if(length == 0 || buf[length - 1] != '\n') buf[length++] = '\n';
	buf[length] = '\0';
	std::fclose(fp);
	
	Token *tks = lexer(buf);

	nowToken = tks;

	// Parsing
	this->parse(tks);

	return true;
}

MITXMLElement *MITXMLDocument::selectRootNode() const {
	return this->root;
}

bool MITXMLDocument::consume(const char *op) {
	if(nowToken->kind != TK_RESERVED || strlen(op) != nowToken->len || strncmp(nowToken->str, op, nowToken->len)) return false;
	nowToken = nowToken->next;
	return true;
}

Token *MITXMLDocument::consumeIdentifier() {
	if(nowToken->kind != TK_IDENT) return nullptr;
	Token *oldToken = nowToken;
	nowToken = nowToken->next;
	return oldToken;
}

void MITXMLDocument::expect(const char *op) {
	if(nowToken->kind != TK_RESERVED || strlen(op) != nowToken->len || strncmp(nowToken->str, op, nowToken->len)) {
		std::exit(1);
	}
	nowToken = nowToken->next;
}

Token *MITXMLDocument::expectIdentifier() {
	if(nowToken->kind != TK_IDENT) std::exit(1);
	Token *oldToken = nowToken;
	nowToken = nowToken->next;
	return oldToken;
}

Token *MITXMLDocument::expectElementVal() {
	if(nowToken->kind != TK_ELEMENTVAL) std::exit(1);
	Token *oldToken = nowToken;
	nowToken = nowToken->next;
	return oldToken;
}

bool MITXMLDocument::isEOF() {
	return nowToken->kind == TK_EOF;
}

void MITXMLDocument::parse(Token *token) {
	expect("<");
	Token *tk = expectIdentifier();
	this->root = new MITXMLElement(std::string(tk->str, tk->len));

	MITXMLElement *currentPtr = this->root;
	MITXMLElement *currentBuf;

	/* The processing after "<root " needs to be changed depending on the type of the root tag.
	 * So, the latter part is processed like a normal tag.
	 */
	goto PARSE_ROOT_END;

	while(!isEOF()) {
		expect("<");
		tk = consumeIdentifier();
		if(tk != nullptr) {			// <hoge>
			currentBuf = currentPtr;
			currentPtr = new MITXMLElement(std::string(tk->str, tk->len));
			currentPtr->parent = currentBuf;
			currentBuf->children.emplace_back(currentPtr);
PARSE_ROOT_END:

			// attributes
			tk = consumeIdentifier();
			while(tk != nullptr) {
				expect("=");
				Token *elemVal = expectElementVal();
				currentPtr->attributes[std::string(tk->str, tk->len)] = std::string(elemVal->str, elemVal->len);
				tk = consumeIdentifier();
			}

			if(consume("/")) {		// <hoge />
				expect(">");
				if(currentPtr->parent != nullptr) currentPtr = currentPtr->parent;
				continue;
			}
			expect(">");
		} else {
			expect("/");				//	</hoge>
			expectIdentifier();
			expect(">");
			if(currentPtr->parent != nullptr) currentPtr = currentPtr->parent;
		}
	}
}

char checkSingleletterReserved(char p) {
	// Punctutor.
	static char spuncts[] = {'"', '<', '>', '/', '='};
	for(int i = 0; i < sizeof(spuncts) / sizeof(char); i++) {
		if(p == spuncts[i]) return spuncts[i];
	}
	return 0;
}

Token::Token(): kind(TK_UNKNOWN), next(nullptr), str(nullptr), len(0) {
}

Token::Token(TokenKind kind, char *str, int len): kind(kind), str(str), len(len), next(nullptr) {
}

// lexer
Token *lexer(char *p) {
	Token head;
	head.next = nullptr;
	Token *current = &head;

	while(*p) {
		// Skip space.
		if(std::isspace(*p)) {
			p++;
			continue;
		}

		// Skip block comment.
		if(!strncmp(p, "<!--", 4)) {
			char *q = strstr(p + 4, "-->");
			p = q + 3;
			continue;
		}

		// Skip xml declaration
		if(!strncmp(p, "<?", 2)) {
			char *q = strstr(p + 2, ">");
			p = q + 1;
			continue;
		}

		char c = checkSingleletterReserved(*p);

		// element
		if(c == '"') {
			p++;
			char *s = p;
			do {
				c = *p++;
			} while(c != '"');
			int len = p - s - 1;
			current->next = new Token(TK_ELEMENTVAL, s, len);
			current = current->next;
			continue;
		}

		if(c != 0) {
			current->next = new Token(TK_RESERVED, p++, 1);
			current = current->next;
			continue;
		}

		if(isalpha(*p) || *p == '_') {
			char *q = p++;
			while(isalnum(*p) || *p == '_') p++;
			current->next = new Token(TK_IDENT, q, p - q);
			current = current->next;
			continue;
		}
	}
	current->next = new Token(TK_EOF, p, 1);
	return head.next;
}
