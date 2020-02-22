#ifndef NODES_H
#define NODES_H

#include "token.h"
#include <algorithm>

using Iterator = std::string::iterator;

struct EvalResult
{
    EvalResult(bool result):
        result(result)
    {}

    EvalResult(bool result, Iterator leaf_pos):
        result(result), leaf_pos(leaf_pos)
    {}

    EvalResult& operator=(const EvalResult& rhs)
    {
        result = rhs.result;
        leaf_pos = rhs.leaf_pos;

        return *this;
    }

    operator bool() const
    {
        return result;
    }
    
    std::vector<std::string> found_groups;

    Iterator first_found;
    Iterator last_found;

    bool result;

    Iterator leaf_pos;
};

struct ASTNode
{
    virtual EvalResult eval(Iterator& curr_pos, Iterator str_end)
    {
        for(auto child: children)
            if(!child->eval(curr_pos, str_end))
                return false;

        return true; 
    }

    virtual void print()
    {
        static int depth = 0;

        depth++;

        for(auto child: children)
        {
            for(int i = 0; i < depth; i++) std::cout << "|\t";

            child->print();
        }

        depth--;
    }

    void addChild(ASTNode* node)
    {
        children.push_back(node);
    }

    std::vector<ASTNode*> children;
};

struct ExprNode: public ASTNode
{
    void print() override
    {
        std::cout << "Expression\n";
        ASTNode::print();
    }
};

struct OpNode: public ASTNode
{
    void print() override
    {
        std::cout << "Operand\n";
        ASTNode::print();
    }
};

struct StrNode: public ASTNode
{
    void print() override
    {
        std::cout << "String\n";
        ASTNode::print();
    }

    bool empty()
    {
        return children.empty();
    }
};

struct CharNode: public ASTNode
{
    CharNode(char value):
        value(value)
    {}

    EvalResult eval(Iterator& curr_pos, Iterator str_end) override
    {
        return *curr_pos == value && curr_pos++ != str_end;
    }

    void print() override
    {
        std::cout << value << "\n";
        ASTNode::print();
    }

    char value;
};

struct GroupNode: public ASTNode
{
    void print() override
    {
        std::cout << "()\n";
        ASTNode::print();
    }
};

struct OrNode: public ASTNode
{
    void print() override
    {
        std::cout << "+\n";
        ASTNode::print();
    }

    EvalResult eval(Iterator& curr_pos, Iterator str_end) override
    {
        if(children.size() != 2) 
        {
            std::cerr << "Too few arguments for or operator\n";
            return false;
        }

        if(children[0]->eval(curr_pos, str_end)) return true;
        else                                     return children[1]->eval(curr_pos, str_end);
    }
};

struct CountNode: public ASTNode
{
    void print() override
    {
        std::cout << "{" << value << "}\n";
        ASTNode::print();
    }

    int value;
};

struct StarNode: public ASTNode
{
    void print() override
    {
        std::cout << "*\n";
        ASTNode::print();
    }
};

struct DotNode: public ASTNode
{
    void print() override
    {
        std::cout << ".\n";
        ASTNode::print();
    }
};

struct GreedyNode: public ASTNode
{
    void print() override
    {
        std::cout << "greedy\n";
        ASTNode::print();
    }
};

struct IgCapsNode: public ASTNode
{
    void print() override
    {
        std::cout << "\\I\n";
        ASTNode::print();
    }
};

#endif