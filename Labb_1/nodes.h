#ifndef NODES_H
#define NODES_H

#include "token.h"
#include <algorithm>

/*
    Prgrm:  Oändlig mängd barn
    Str:    Inga barn
    +:      ett eller två barn
    *:      ett eller två barn (vilken operand som ska upprepas, samt potentiell vidare text)
    ():     ?
    {}:     ett barn
    \I:     
    \O{}:   
*/

/*
    lo* could.{3}

         *
        / \
       lo   .
        /        \
    could {3}


    promise to (Love+Hate) you

*/

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
    
    bool result;

    Iterator leaf_pos;
};

struct Node
{
    bool eval(Iterator str_begin, Iterator str_end)
    {
        Iterator curr_pos = str_begin;

        return eval(str_begin, str_end, curr_pos);
    };

    virtual EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos)
    {
        std::cout << "DENNA SKA EJ KÖRAS\n";

        return false; 
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

    void addChild(Node* node)
    {
        children.push_back(node);
    }

    std::vector<Node*> children;
};

struct ProgramNode: public Node //Base node
{
    void print() override
    {
        std::cout << "Program\n";
        Node::print();
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        if(children.size() == 0) return false;

        return children[0]->eval(str_begin, str_end, curr_pos);
    }
};

struct RegexNode: public Node
{
    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        if(str_begin >= str_end && children.size() == 0) return false;

        EvalResult childEval = children[0]->eval(str_begin, str_end, curr_pos);

        if(childEval)   return true; 
        else            return eval(std::next(childEval.leaf_pos), str_end, curr_pos);
    }

    void print() override
    {
        std::cout << "Regex\n";
        Node::print();
    }
};

struct CharNode: public Node
{
    CharNode(char value):
        value(value)
    {}

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        if(str_begin >= str_end) return false;

        if(children.size() != 0)
        {
            EvalResult childEval = children[0]->eval(str_begin, str_end, curr_pos);
            
            if(!childEval)  return EvalResult(false, childEval.leaf_pos);
            else            return EvalResult(*--curr_pos == value, childEval.leaf_pos);
        }
        else
        {
            //std::cout << *str_begin << "\n";

            curr_pos = std::find_if(str_begin, str_end, [&](const char it)
            {
                return it == value;
            });

            return EvalResult(curr_pos != str_end, curr_pos);
        }
    }

    void print() override
    {
        std::cout << value << "\n";
        Node::print();
    }

    char value;
};

struct ParenNode: public Node
{
    void print() override
    {
        std::cout << "()\n";
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        return false;
    }

    bool ignoreCaps = false;
};

struct OrNode: public Node
{
    void print() override
    {
        std::cout << "+\n";
        Node::print();
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        if(children.size() != 2) 
        {
            std::cerr << "Too few arguments for or operator\n";
            return false;
        }

        if(children[0]->eval(str_begin, str_end, curr_pos)) return true;
        else                                                return children[1]->eval(str_begin, str_end, curr_pos);
    }
};

struct StarNode: public Node
{
    void print() override
    {
        std::cout << "*\n";
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        return false;
    }
};

struct DotNode: public Node
{
    void print() override
    {
        std::cout << ".\n";
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        return false;
    }
};

#endif