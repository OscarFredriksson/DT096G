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

    EvalResult& operator=(const EvalResult& rhs)
    {
        result = rhs.result;

        return *this;
    }

    operator bool() const
    {
        return result;
        //return first_found != last_found;
    }
    
    std::vector<std::string> found_groups;

    Iterator first_found;
    Iterator last_found;

    bool result;
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
    /*EvalResult eval(Iterator& curr_pos, Iterator str_end) override
    {
        ASTNode::eval(curr_pos, str_end);
    }*/

    void print() override
    {
        std::cout << "EXPRESSION\n";
        ASTNode::print();
    }
};

struct OpNode: public ASTNode
{
    void print() override
    {
        std::cout << "OPERAND\n";
        ASTNode::print();
    }
};

struct SubOpNode: public ASTNode
{
    void print() override
    {
        std::cout << "SUB-OPERAND\n";
        ASTNode::print();
    }
};

struct StrNode: public ASTNode
{
    EvalResult eval(Iterator& curr_pos, Iterator str_end) override
    {
        while(curr_pos != str_end)
            if(ASTNode::eval(curr_pos, str_end)) 
                return true;  

        return false;
    }

    void print() override
    {
        std::cout << "STRING\n";
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
        return *curr_pos++ == value;
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
        std::cout << "GROUP\n";
        ASTNode::print();
    }
};

struct OrNode: public ASTNode
{
    void print() override
    {
        std::cout << "OR\n";
        ASTNode::print();
    }

    EvalResult eval(Iterator& curr_pos, Iterator str_end) override
    {
        if(children.size() != 2) 
        {
            std::cerr << "Too few arguments for or operator\n";
            return false;
        }

        Iterator temp_it = curr_pos;

        if(children[0]->eval(temp_it, str_end)) return true;
        else                                    return children[1]->eval(curr_pos, str_end);
    }
};

struct CountNode: public ASTNode
{
    CountNode(int value):
        value(value)
    {}

    void print() override
    {
        std::cout << "COUNT" << value << "\n";
        ASTNode::print();
    }

    int value;
};

struct StarNode: public ASTNode
{
    /*EvalResult eval(Iterator& curr_pos, Iterator end) override
    {
        


        return true;
    }*/
    
    void print() override
    {
        std::cout << "STAR\n";
        ASTNode::print();
    }
};

struct DotNode: public ASTNode
{
    EvalResult eval(Iterator& curr_pos, Iterator end) override
    {
        return curr_pos++ != end;
    }

    void print() override
    {
        std::cout << "DOT\n";
        ASTNode::print();
    }
};

struct GreedyNode: public ASTNode
{
    void print() override
    {
        std::cout << "GREEDY\n";
        ASTNode::print();
    }
};

struct IgCapsNode: public ASTNode
{
    void print() override
    {
        std::cout << "IGNORE-CAPS\n";
        ASTNode::print();
    }
};

#endif