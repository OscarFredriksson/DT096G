#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum TokenType
{
    CHAR,
    OR,
    STAR,
    LEFT_PAREN,
    RIGHT_PAREN,
    COUNTER,
    DOT,
    IGNORE_CAPS,
    OUTPUT_GROUP
};

class Token
{
public:
    Token(TokenType type):
        type(type)
    {};

    void print()
    {
        std::cout << " TOKEN: ";

        switch(type)
        {
            case TokenType::CHAR:                   std::cout << value;
                                                    break;
            case TokenType::OR:                     std::cout << "+";
                                                    break;
            case TokenType::STAR:                   std::cout << "*";
                                                    break;
            case TokenType::LEFT_PAREN:             std::cout << "(";
                                                    break;
            case TokenType::RIGHT_PAREN:            std::cout << ")";
                                                    break;
            case TokenType::COUNTER:                std::cout << "{" + std::string(1, value) + "}";
                                                    break;
            case TokenType::DOT:                    std::cout << ".";
                                                    break;
            case TokenType::IGNORE_CAPS:            std::cout << "\\I";
                                                    break;
            case TokenType::OUTPUT_GROUP:           std::cout << "\\O{" + std::string(1, value) + "}";
            default:                                break;
        }
    }

    TokenType type;
    char value; 
};

#endif