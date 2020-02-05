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
            case TokenType::STRING:         token.val = parseStr();
                                            break;
            case TokenType::OUTPUT_GROUP:   it++;
            case TokenType::COUNTER:        token.val = parseBraces();   
                                            break; 
            default:                        it++;
        }
        
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

std::string Parser::parseBraces()
{
    if(*it != '{')  throw std::runtime_error("ERROR: Expected { sign.");

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
        case '\\':  
        {
            token_c = *++it;
            if(token_c == 'I') return TokenType::IGNORE_CAPS;
            if(token_c == 'O') return TokenType::OUTPUT_GROUP;
        }

        default: throw std::runtime_error("Error: Unknown identifier");
    }
}