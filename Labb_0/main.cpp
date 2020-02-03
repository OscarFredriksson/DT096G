#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <vector>

//#include "tokens.h"

enum TokenType
{
    Integer,
    Plus,
    Minus
};

struct Token
{
    Token(TokenType type):
        type(type)
    {}

    TokenType type;
    int value;
};

TokenType getOp(char c)
{
    switch(c)
    {
        case '+':   return TokenType::Plus;
        case '-':   return TokenType::Minus;
        //case '*':   return Operator::Mult;
        //case '/':   return Operator::Div;
        default:    throw std::runtime_error("Error: Undefined operator");
    }
}

/*int calc(int first, int second, Operator op)
{
    switch(op)
    {
        case Operator::Plus:    return first + second;
        case Operator::Minus:   return first - second;
        case Operator::Div:     return first / second;
        case Operator::Mult:    return first * second;
    }
}*/

int charToInt(char c)
{
    return c - '0';
}

void trimLeadingWhitespace(std::string::iterator& it)
{
    while(isspace(*it)) it++;
}

int parseInt(std::string::iterator& it)
{
    trimLeadingWhitespace(it);

    if(isdigit(*it)) 
    {
        std::string leftStr = "";
        
        for(; isdigit(*it); it++) leftStr.push_back(*it);

        return std::stoi(leftStr);
    }
    else throw std::runtime_error("Error: Found unexpected non-integer.\n");       
}

TokenType parseOperator(std::string::iterator& it)
{
    trimLeadingWhitespace(it);
    TokenType op = getOp(*it);
    it++;
    return op;
}

/*void old_parse()
{
    std::string line;

    while(std::getline(std::cin, line))
    {
        if(line == "exit") return;

        std::string::iterator it = line.begin();
        int sum;
        Operator op;

        for(int i = 0; it != line.end(); i++)
        {
            if(i % 2 == 0)
            {
                try
                {
                    if(i == 0)  sum = parseInt(it);
                    else        
                    {
                        int val = parseInt(it); 
                        sum = calc(sum, val, op);
                    }  
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    goto readline;
                }
            }
            else
            {
                try
                {
                    op = parseOperator(it);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    goto readline;
                }
            } 
        }

        std::cout << sum << "\n";
        readline:;
    }
}*/


std::vector<Token> tokenize(std::string line)
{
    std::vector<Token> tokens;

    std::string::iterator it = line.begin();

    for(int i = 0; it != line.end(); i++)
    {
        trimLeadingWhitespace(it);

        if(isdigit(*it))    
        {
            try
            {                
                int val = parseInt(it);
                Token token(TokenType::Integer);
                token.value = val;
                tokens.push_back(token);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else                
        {
            try
            {
                TokenType type = parseOperator(it);

                tokens.push_back(Token(type));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }    
        }
    }

    return tokens;
}

struct Node
{
    virtual int eval() = 0;
};

struct IntNode: public Node
{
    IntNode(int value):
        value(value)
    {}

    int eval() override
    {
        return value;
    }

    int value;
};

struct AddNode: public Node
{
    IntNode* leftChild;
    IntNode* rightChild;

    int eval() override
    { 
        return leftChild->eval() + rightChild->eval(); 
    }
};

Node* buildTree(const std::vector<Token>& tokens)
{
    auto it = tokens.begin();

    int int1 = it->value;
    it++;

    if(it->type != TokenType::Plus) return nullptr;

    AddNode* op = new AddNode();
    it++;

    int int2 = it->value;
    it++;

    op->leftChild = new IntNode(int1);
    op->rightChild = new IntNode(int2);

    return op;
}

int main(int argc, char* argv[])
{
    //parse();

    std::string line;
    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

        std::vector<Token> tokens = tokenize(line);

        Node* root = buildTree(tokens);

       std::cout << root->eval() << "\n";
    }

    return 0;
}