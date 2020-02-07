#ifndef LEXER_H
#define LEXER_H

#include <vector>

#include "token.h"

//Ska vara recursive descent

class Lexer
{
public:
    Lexer(std::string str);

    std::vector<Token> tokenize();

    static bool isIdentifier(char c);

private:    
    char parseBraces();

    TokenType getTokenType();

    std::string str;
    std::string::iterator it;
};

#endif