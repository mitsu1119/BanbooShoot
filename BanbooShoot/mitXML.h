#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

void strSplit(std::string str, char ch, std::vector<std::string> &res);
void trim(std::string &s);

class MITXMLDocument;
class MITXMLNodeList {
	friend MITXMLDocument;
private:
	std::string tagName;
	MITXMLNodeList *parent;
	std::vector<MITXMLNodeList *> children;

public:
	MITXMLNodeList(std::string tagName);
};


class MITXMLDocument {
private:
	MITXMLNodeList *root;

public:
	MITXMLDocument(const char *fileName);
	~MITXMLDocument();
	
	bool load(const char *fileName);
};
