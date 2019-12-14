#include "mitXML.h"

MITXMLNodeList::MITXMLNodeList(std::string tagName): tagName(tagName), parent(nullptr) {
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
	// load root tag
	while(getline(ifs, buf)) {
		trim(buf);
		if(buf == "") continue;		// skip white line
		strSplit(buf, ' ', elems);
		elems[0].erase(std::begin(elems[0]));
		if(elems[0][elems[0].size() - 1] == '>') elems[0].pop_back();
		this->root = new MITXMLNodeList(elems[0]);
		currentPtr = this->root;
		std::vector<std::string>().swap(elems);
		break;
	}

	MITXMLNodeList *currentBuf;
	while(getline(ifs, buf)) {
		trim(buf);
		if(buf == "") continue;		// skip white line
		strSplit(buf, ' ', elems);
		if(elems[0][1] == '/') {
			if(currentPtr->parent == nullptr) {
				std::vector<std::string>().swap(elems);
				break;
			}
			currentPtr = currentPtr->parent;
		} else {
			if(elems[elems.size() - 1][elems[elems.size() - 1].size() - 2] == '/') {			// <hoge/>
				elems[0].erase(std::begin(elems[0]));
				if(elems[0][elems[0].size() - 1] == '>') elems[0].pop_back();
				if(elems[0][elems[0].size() - 1] == '/') elems[0].pop_back();
				currentBuf = new MITXMLNodeList(elems[0]);
				currentPtr->children.emplace_back(currentBuf);
				currentBuf->parent = currentPtr;
			} else {																										// <hoge> </hoge>
				elems[0].erase(std::begin(elems[0]));
				if(elems[0][elems[0].size() - 1] == '>') elems[0].pop_back();
				currentBuf = currentPtr;
				currentPtr = new MITXMLNodeList(elems[0]);
				currentPtr->parent = currentBuf;
				currentBuf->children.emplace_back(currentPtr);
			}
		}
		std::vector<std::string>().swap(elems);
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

void trim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}
