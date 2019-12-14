#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

void strSplit(std::string str, char ch, std::vector<std::string> &res);
void trim(std::string &s);

// Token data.
typedef enum {
	TK_RESERVED,
	TK_IDENT,
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

/*	
all = "<!--" STR '-->
		| '<' STR '/' '>'
*/
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
