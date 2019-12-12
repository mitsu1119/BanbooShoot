#pragma once
#include <fstream>
#include <vector>
#include <string>

void strSplit(std::string str, char ch, std::vector<std::string> &res);

class MITXMLNodeList {
private:
	std::string tagName;

public:
	MITXMLNodeList(std::string tagName);
};


class MITXMLDocument {
private:
	MITXMLNodeList *child;

public:
	MITXMLDocument(const char *fileName);
	~MITXMLDocument();
	
	bool load(const char *fileName);
};
