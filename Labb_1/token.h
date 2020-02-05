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
    IGNORE_CAPS
};

class Token
{
public:
    Token(TokenType type, std::string str = ""):
        type(type), str(str)
    {};

    void print()
    {
        std::cout << " TOKEN: ";

        switch(type)
        {
            case TokenType::STRING:                 std::cout << str;
                                                    break;
            case TokenType::OR:                     std::cout << "+";
                                                    break;
            case TokenType::ANY:                    std::cout << "*";
                                                    break;
            case TokenType::LEFT_PAREN:             std::cout << "(";
                                                    break;
            case TokenType::RIGHT_PAREN:            std::cout << ")";
                                                    break;
            case TokenType::COUNTER:                std::cout << "{" + str + "}";
                                                    break;
            case TokenType::DOT:                    std::cout << ".";
                                                    break;
            case TokenType::IGNORE_CAPS:            std::cout << "\\I";
                                                    break;
            default:                                break;
        }
    }

    TokenType type;
    std::string str; 
};

#endif