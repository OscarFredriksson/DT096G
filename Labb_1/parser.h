#ifndef PARSER_H
#define PARSER_H

#include "token.h"

class Parser
{
public:
    Parser(std::string str):
        str(str)
    {
        it = this->str.begin();
    }

std::vector<Token> tokenize()
{
    std::vector<Token> tokens;

    std::cout << str << "\n";

    while(it != str.end())
    {
        Token token = Token(getTokenType());
        
        if(token.type == TokenType::STRING) token.str = parseStr();        
        else                                it++;
        
        tokens.push_back(token);
    }

    return tokens;
}

private:
    bool isIdentifier(char c)
    {
        return isalpha(c) || isdigit(c) || c == ' ';
    }

    std::string parseStr()
    {
        if(isIdentifier(*it)) 
        {
            std::string str = "";
            
            for(; isIdentifier(*it); it++) str.push_back(*it);

            return str;
        }
        else throw std::runtime_error("Error: Found unexpected identifier.\n");       
    }

    TokenType getTokenType()
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

            }
            default:    throw std::runtime_error("ERROR: Unknown identifier");
        }
    }

    std::string str;
    std::string::iterator it;
};

#endif