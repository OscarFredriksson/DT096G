#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>

enum TokenType
{
    STRING,
    OR,
    ANY,
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
    Token(TokenType type, std::string val = ""):
        type(type), val(val)
    {};

    void print()
    {
        std::cout << " TOKEN: ";

        switch(type)
        {
            case TokenType::STRING:                 std::cout << val;
                                                    break;
            case TokenType::OR:                     std::cout << "+";
                                                    break;
            case TokenType::ANY:                    std::cout << "*";
                                                    break;
            case TokenType::LEFT_PAREN:             std::cout << "(";
                                                    break;
            case TokenType::RIGHT_PAREN:            std::cout << ")";
                                                    break;
            case TokenType::COUNTER:                std::cout << "{" + val + "}";
                                                    break;
            case TokenType::DOT:                    std::cout << ".";
                                                    break;
            case TokenType::IGNORE_CAPS:            std::cout << "\\I";
                                                    break;
            case TokenType::OUTPUT_GROUP:           std::cout << "\\O{" + val + "}";
            default:                                break;
        }
    }

    TokenType type;
    std::string val; 
};

#endif