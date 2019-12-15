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

// bnf
/*
root := '<' STR '>' program '<' '/' STR '>'
program := ('<' STR '>' program '<' '/' STR '>')*
*/
class MITXMLDocument {
private:
	MITXMLNodeList *root;
	Token *nowToken;

	// If next token is expected reserved word, read token and return true.
	bool consume(const char *op);
	Token *consumeIdentifier();

	// If next token is expected reserved word, read token. Otherwise output error.
	void expect(const char *op);
	Token *expectIdentifier();

	bool isEOF();

	void parse(Token *root);

public:
	MITXMLDocument(const char *fileName);
	~MITXMLDocument();
	
	bool load(const char *fileName);
};
