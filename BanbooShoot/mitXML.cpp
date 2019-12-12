#include "mitXML.h"

MITXMLNodeList::MITXMLNodeList(std::string tagName): tagName(tagName) {
}

std::ifstream ifs;
MITXMLDocument::MITXMLDocument(const char *fileName) {
	this->load(fileName);
}

MITXMLDocument::~MITXMLDocument() {
	delete this->child;
}

bool MITXMLDocument::load(const char *fileName) {
	std::string buf;
	ifs.open(fileName);
	getline(ifs, buf);		// skip the first line

	std::vector<std::string> elems;
	if(getline(ifs, buf)) {
		strSplit(buf, ' ', elems);
		elems[0].erase(std::begin(elems[0]));
		this->child = new MITXMLNodeList(elems[0]);
	}

	return true;
}

void strSplit(std::string str, char ch, std::vector<std::string> &res) {	
	char charactor;
	int currentPos = 0;
	std::string buf = "";

	while((unsigned int)currentPos < str.size()) {
		charactor = str[currentPos++];
		if(currentPos == str.size()) {
			buf += charactor;
			res.emplace_back(buf);
			break;
		}
		if(charactor != ch) buf += charactor;
		else {
			res.emplace_back(buf);
			buf = "";
		}
	}
}
