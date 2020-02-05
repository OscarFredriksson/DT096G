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
        
        switch(token.type)
        {
            case TokenType::STRING:     token.str = parseStr();
                                        break;
            case TokenType::COUNTER:    token.str = parseCounter();   
                                        break; 
            default:                    it++;
                                        break;
        }

        /*if(token.type == TokenType::STRING) token.str = parseStr();        
        else if(token.type ==)
        else                                it++;*/
        
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
    std::string str = "";
    
    while(isIdentifier(*it)) str.push_back(*it++);

    return str; 
}

std::string Parser::parseCounter()
{
    if(!std::isdigit(*++it)) throw std::runtime_error("ERROR: Found non integer as counter value");

    std::string value(1, *it++);

    if(!*it++ == '}') throw std::runtime_error("ERROR: Found no closing counter brace");

    return value;
}

TokenType Parser::getTokenType()
{
    char token_c = *it;

    if(isIdentifier(token_c))  return TokenType::STRING;

    switch(token_c)
    {
        case '+':   return TokenType::OR;
        case '*':   return TokenType::ANY;
        case '(':   return TokenType::LEFT_PAREN;
        case ')':   return TokenType::RIGHT_PAREN;
        case '.':   return TokenType::DOT; 
        case '{':   return TokenType::COUNTER;
        case '\\':  if(*++it == 'I') return TokenType::IGNORE_CAPS;
        default: throw std::runtime_error("Error: Unknown identifier");

    }
}