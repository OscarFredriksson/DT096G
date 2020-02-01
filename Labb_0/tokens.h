#include <iostream>

enum Operator
{
    Plus,
    Minus, 
    Div,
    Mult
};

struct Token
{
    enum Type
    {
        Integer,
        Operator
    };

    Type type;

    virtual void print() = 0;
};

struct IntegerToken: public Token
{
    IntegerToken(int value):
        value(value)
    {}

    int value;

    void print() override
    {
        std::cout << value << " ";
    }
};

struct OperatorToken: public Token
{
    ::Operator value;

    OperatorToken(::Operator value):
        value(value)
    {}

    void print() override
    {
        switch(value)
        {
            case Plus:  std::cout << "+ ";
                        break;
            case Minus: std::cout << "- ";
                        break;
            case Div:   std::cout << "/ ";
                        break;
            case Mult:  std::cout << "* ";
                        break;
        }
    }
};
