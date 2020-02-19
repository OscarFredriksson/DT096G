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
    
    std::vector<std::string> found_groups;

    Iterator first_found;
    Iterator last_found;

    bool result;

    Iterator leaf_pos;
};

struct Node
{
    bool eval(Iterator str_begin, Iterator str_end)
    {
        Iterator curr_pos = str_begin;

        for(; curr_pos != str_end; curr_pos++)
        {
            if(eval(str_begin, str_end, curr_pos)) return true;
        }

        return false;
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

struct ExprNode: public Node //Base node
{
    void print() override
    {
        std::cout << "Expression\n";
        Node::print();
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        if(children.size() == 0) return false;

        /*for(auto c: children)
        {
            c->eval(str_begin, str_end, curr_pos);
        }*/


        return children[0]->eval(str_begin, str_end, curr_pos);
    }

    /*void addChild(Node* node) override
    {
        if(children.size() == 0)    return Node::addChild(node);

        ExprNode* exprNode = new ExprNode();

        exprNode->Node::addChild(node);

        return exprNode;
    }*/
};

struct StrNode: public Node
{
    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        for(auto c: children)
        {
            if(!c->eval(str_begin, str_end, curr_pos)) return false;

            curr_pos++;
        }

        return true;
    }

    void print() override
    {
        std::cout << "Str\n";
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
        return *curr_pos == value && curr_pos != str_end;;
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
        Node::print();
    }

    EvalResult eval(Iterator str_begin, Iterator str_end, Iterator& curr_pos) override
    {
        return false;
    }
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