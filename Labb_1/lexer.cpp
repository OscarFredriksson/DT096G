#include "lexer.h"

#include <stdexcept>

 Lexer::Lexer(std::string str):
    str(str)
{
    it = this->str.begin();
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while(it != str.end())
    {
        Token token = Token(getTokenType());
        
        switch(token.type)
        {
            case TokenType::CHAR:           token.value = *it++;
                                            break;
            case TokenType::OUTPUT_GROUP:   it++;
            case TokenType::COUNTER:        token.value = parseBraces();   
                                            break; 
            default:                        it++;
        }
        
        tokens.push_back(token);
    }

    return tokens;
}

bool Lexer::isIdentifier(char c)
{
    return  (isalpha(c) || isdigit(c) || c == ' ');
}

char Lexer::parseBraces()
{
    if(*it != '{')  throw std::runtime_error("ERROR: Expected { sign.");

    if(!std::isdigit(*++it)) throw std::runtime_error("ERROR: Found non integer as counter value");

    char value = *it++;

    if(*it++ != '}') throw std::runtime_error("ERROR: Found no closing counter brace");

    return value;
}

TokenType Lexer::getTokenType()
{
    char token_c = *it;

    if(isIdentifier(token_c))  return TokenType::CHAR;

    switch(token_c)
    {
        case '+':   return TokenType::OR;
        case '*':   return TokenType::STAR;
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