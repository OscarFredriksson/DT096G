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
    }
    
    bool result;
};

struct Operands
{
    Operands(Iterator& begin, Iterator end):
        begin(begin), end(end)
    {}

    Operands& operator=(const Operands& rhs)
    {
        begin = rhs.begin;
        end = rhs.end;

        igCaps = rhs.igCaps;

        return *this;
    }

    std::vector<std::string> found_groups;

    Iterator begin;
    Iterator end;

    bool igCaps = false;
};

struct ASTNode
{
    virtual EvalResult eval(Operands& input)
    {
        for(auto child: children)
            if(!child->eval(input))
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

struct PrgmNode: public ASTNode
{
    EvalResult eval(Operands& input) override
    {
        while(input.begin != input.end)
        {
            bool match = true;

            input.found_groups.clear();

            input.found_groups.push_back("");

            for(auto child: children)
            {
                if(!child->eval(input)) match = false;
            }

            if(match)   return true;
        }

        return false;
    }

    void print() override
    {
        std::cout << "PROGRAM\n";
        ASTNode::print();
    }
};

struct ExprNode: public ASTNode
{
    EvalResult eval(Operands& input) override
    {
        for(auto child: children)
        {
            if(!child->eval(input)) return false;
        }

        return true;
    }

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

    EvalResult eval(Operands& input) override
    {   
        if(input.begin == input.end) return false;

        bool match;

        if(input.igCaps)
            match = std::tolower(*input.begin) == std::tolower(value);
        else
            match = *input.begin == value;

        if(match)   input.found_groups[0] += *input.begin;

        input.begin++;

        return match;
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
    EvalResult eval(Operands& input) override
    {
        input.found_groups.push_back("");

        return ASTNode::eval(input);
    }

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

    EvalResult eval(Operands& input) override
    {
        Operands temp = input;

        if(children[0]->eval(temp)) 
        {
            input.begin = temp.begin;
            return true;
        }
        else return children[1]->eval(input);
    }
};

struct CountNode: public ASTNode
{
    CountNode(int value):
        value(value)
    {}

    EvalResult eval(Operands& input) override
    {
        for(int i = 0; i < value; i++)
        {
            if(!children[0]->eval(input)) return false;
        }

        return true;
    }

    void print() override
    {
        std::cout << "COUNT " << value << "\n";
        ASTNode::print();
    }

    int value;
};

struct StarNode: public ASTNode
{
    EvalResult eval(Operands& input) override
    {
        if(!children[0]->eval(input)) return false;

        while(children[0]->eval(input));
        
        input.begin--;    //Loopen går ett varv för långt när den blir false

        return true;
    }
    
    void print() override
    {
        std::cout << "STAR\n";
        ASTNode::print();
    }
};

struct DotNode: public ASTNode
{
    EvalResult eval(Operands& input) override
    {
        bool match = input.begin != input.end;

        if(match) input.found_groups[0] += *input.begin;

        input.begin++;

        return match;
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
    EvalResult eval(Operands& input) override
    {
        input.igCaps = true;

        bool ret = ASTNode::eval(input);

        input.igCaps = false;

        return ret;
    }

    void print() override
    {
        std::cout << "IGNORE-CAPS\n";
        ASTNode::print();
    }
};

#endif