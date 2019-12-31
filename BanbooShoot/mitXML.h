#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>

// Token data.
typedef enum {
	TK_RESERVED,
	TK_IDENT,
	TK_ELEMENTVAL,
	TK_EOF,
	TK_UNKNOWN
} TokenKind;

// Token type
class Token {
public:
	Token();
	Token(TokenKind kind, char *str, int len);

	TokenKind kind;
	Token *next;
	char *str;
	int len;
};

Token *lexer(char *p);

/* MITXMLElement class
 * This class means each tag.
 */
class MITXMLDocument;
class MITXMLNodeList;
class MITXMLElement {
	friend MITXMLDocument;
private:
	std::string tagName;
	MITXMLElement *parent;
	std::vector<MITXMLElement *> children;
	std::unordered_map<std::string, std::string> attributes;

public:
	MITXMLElement(std::string tagName);

	/* get attribute value named @attrName */
	std::string getAttribute(std::string attrName) const;

	/* return node list */
	// Search only root level.
	MITXMLNodeList selectNodes(std::string nodeName) const;
};

/* MITXMLNodeList class
 * This class have many same name tags.
 */
class MITXMLNodeList {
private:
public:
	std::vector<MITXMLElement *> item;
	size_t length() const;
};

/* bnf
 * root := (program)+
 * program := '<' STR (STR '=' '"' STR '"')* '>' program '<' '/' STR '>'
 *                  | '<' STR (STR '=' '"' STR '"')* '/' '>' 
 */
class MITXMLDocument {
private:
	MITXMLElement *root;
	Token *nowToken;

	// If next token is expected reserved word, read token and return true.
	bool consume(const char *op);
	Token *consumeIdentifier();

	// If next token is expected reserved word, read token. Otherwise output error.
	void expect(const char *op);
	Token *expectIdentifier();
	Token *expectElementVal();

	bool isEOF();

	void parse(Token *root);

public:
	MITXMLDocument(const char *fileName);
	~MITXMLDocument();
	
	/* parsing and loading */
	bool load(const char *fileName);

	/* return root node */
	MITXMLElement *selectRootNode() const;
};
