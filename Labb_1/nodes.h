#include "token.h"

struct Node
{
    virtual void evaluate() = 0;

    bool evaluated;
};

struct OperatorNode: public Node
{
    void evaluate() override
    {}

    Token token;
    Node* leftChild;
    Node* rightChild;
}

struct StringNode: public Node
{
    void evaluate() override
    {}

    std::string value;

    //Ska alltid vara ett löv, behöver inga child pointers
}