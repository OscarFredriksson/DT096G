#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "token.h"


class Parser
{
public:
    Parser(std::string str);

    std::vector<Token> tokenize();

private:
    bool isIdentifier(char c);
    
    std::string parseStr();

    TokenType getTokenType();

    std::string str;
    std::string::iterator it;
};

#endif