#include "mitXML.h"

MITXMLNodeList::MITXMLNodeList(std::string tagName): tagName(tagName), parent(nullptr), child(nullptr) {
}

std::ifstream ifs;
MITXMLDocument::MITXMLDocument(const char *fileName) {
	this->load(fileName);
}

MITXMLDocument::~MITXMLDocument() {
	delete this->root;
}

bool MITXMLDocument::load(const char *fileName) {
	std::string buf;
	ifs.open(fileName);
	getline(ifs, buf);		// skip the first line

	std::vector<std::string> elems;
	MITXMLNodeList *currentPtr = this->root;
	if(getline(ifs, buf)) {
		strSplit(buf, ' ', elems);
		elems[0].erase(std::begin(elems[0]));
		if(elems[0][elems[0].size() - 1] == '>') elems[0].pop_back();
		this->root = new MITXMLNodeList(elems[0]);
		currentPtr = this->root;
		std::vector<std::string>().swap(elems);
	}

	MITXMLNodeList *currentBuf;
	while(getline(ifs, buf)) {
		strSplit(buf, ' ', elems);
		if(elems[0][1] == '/') {
			if(currentPtr->parent == nullptr) break;
			currentPtr = currentPtr->parent;
		} else {
			elems[0].erase(std::begin(elems[0]));
			if(elems[0][elems[0].size() - 1] == '>') elems[0].pop_back();
			currentBuf = currentPtr;
			currentPtr = new MITXMLNodeList(elems[0]);
			currentPtr->parent = this->root;
			currentBuf->child = currentPtr;
			std::vector<std::string>().swap(elems);
		}
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
