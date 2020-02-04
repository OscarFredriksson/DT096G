#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <vector>

enum TokenType
{
    Plus,
    Minus,
    Integer
};

struct Token
{
    Token(TokenType type):
        type(type)
    {}

    TokenType type;
    int value;
};


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

struct OpNode: public Node
{
    Node* leftChild;
    Node* rightChild;

    TokenType type;

    OpNode(TokenType type):
        type(type)
    {}

    int eval() override
    { 
        if(type == Plus)    return leftChild->eval() + rightChild->eval();
        else                return leftChild->eval() - rightChild->eval();
    }
};

TokenType getTokenType(char c)
{
    if(isdigit(c)) return Integer;

    switch(c)
    {
        case '+':           return TokenType::Plus;
        case '-':           return TokenType::Minus;
        default:            throw std::runtime_error("Error: Undefined operator");
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
    std::string leftStr = "";
    
    for(; isdigit(*it); it++) leftStr.push_back(*it);

    return std::stoi(leftStr);
}

std::vector<Token> tokenize(std::string line)
{
    std::vector<Token> tokens;

    std::string::iterator it = line.begin();
    trimLeadingWhitespace(it);

    while(it != line.end())
    {
        TokenType type = getTokenType(*it);
        Token token(type);
        if(type == Integer)
        {
            int val = parseInt(it);
            token.value = val;
        }
        else it++;

        trimLeadingWhitespace(it);

        tokens.push_back(token);
    }

    return tokens;
}

/*Node* buildTree(const std::vector<Token>& tokens)
{
    auto it = tokens.begin();

    int int1 = it->value;
    it++;

    if(it->type != TokenType::Plus && it->type != TokenType::Minus) return nullptr;

    OpNode* op = new OpNode(it->type);
    it++;

    int int2 = it->value;
    it++;

    op->leftChild = new IntNode(int1);
    op->rightChild = new IntNode(int2);

    return op;
}*/

Node* buildTree(const std::vector<Token>& tokens)
{
    auto token_it = tokens.begin();
    
    OpNode* root = nullptr;
    OpNode* prev_op = nullptr;

    while(token_it != tokens.end())
    {
        auto op_it = std::find_if(token_it, tokens.end(), [](const Token& token)
        {
            return token.type == Plus || token.type == Minus;
        });

        if(op_it == std::end(tokens)) 
        {
            prev_op->rightChild = new IntNode(token_it->value);
            break;
        }

        OpNode* op = new OpNode(op_it->type);

        if(root == nullptr) root = op;
        else                prev_op->rightChild = op;

        op->leftChild = new IntNode(token_it->value);
        prev_op = op;

        token_it = std::next(op_it);
    }

    return root;
}

int main(int argc, char* argv[])
{
    std::string line;
    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

        std::vector<Token> tokens = tokenize(line);

        Node* root = buildTree(tokens);

        //std::cout << "tree built\n";

        std::cout << root->eval() << "\n";
    }

    return 0;
}