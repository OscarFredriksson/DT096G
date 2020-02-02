#include "parser.h"

#include <stdexcept>

 Parser::Parser(std::string str):
    str(str)
{
    it = this->str.begin();
}

std::vector<Token> Parser::tokenize()
{
    std::vector<Token> tokens;

    while(it != str.end())
    {
        Token token = Token(getTokenType());
        
        if(token.type == TokenType::STRING) token.str = parseStr();        
        else                                it++;
        
        tokens.push_back(token);
    }

    return tokens;
}

bool Parser::isIdentifier(char c)
{
    return isalpha(c) || isdigit(c) || c == ' ';
}

std::string Parser::parseStr()
{
    if(isIdentifier(*it)) 
    {
        std::string str = "";
        
        for(; isIdentifier(*it); it++) str.push_back(*it);

        return str;
    }
    else throw std::runtime_error("Error: Found unexpected identifier.\n");       
}

TokenType Parser::getTokenType()
{
    if(isIdentifier(*it))  return TokenType::STRING;

    switch(*it)
    {
        case '+':   return TokenType::OR;
        case '*':   return TokenType::ANY;
        case '(':   return TokenType::LEFT_PAREN;
        case ')':   return TokenType::RIGHT_PAREN;
        case '{':   return TokenType::LEFT_CURLY_BRACKET;
        case '}':   return TokenType::RIGHT_CURLY_BRACKET;
        case '.':   return TokenType::DOT; 
        case '\\':  
        {
            it++;
            if(*it == 'I') return TokenType::IGNORE_CAPS;
        }
        default:    throw std::runtime_error("ERROR: Unknown identifier");
    }
}