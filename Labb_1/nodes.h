#ifndef NODES_H
#define NODES_H

#include "token.h"

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

struct Node
{
    void eval(std::string::iterator str_begin, std::string::iterator str_end)
    {
        this->str_begin = str_begin;
        this->str_end = str_end;

        eval();
    };

    virtual bool eval()
    {
        for(auto c: children)
        {
            c->eval();
        }
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

    std::string::iterator str_begin;
    std::string::iterator str_end;
};

struct ProgramNode: public Node //Base node
{
    void print() override
    {
        std::cout << "Program\n";
        Node::print();
    }

    void eval() override
    {
        //std::for_each(children.begin(), children.end(), eval);
    }
};

struct RegexNode: public Node
{
    void eval() override
    {}

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

    void eval() override
    {}

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

    void eval() override
    {}
};

struct OrNode: public Node
{
    void print() override
    {
        std::cout << "+\n";
        Node::print();
    }

    void eval() override
    {}
};

struct StarNode: public Node
{
    void print() override
    {
        std::cout << "*\n";
    }

    void eval() override
    {}
};

struct DotNode: public Node
{
    void print() override
    {
        std::cout << ".\n";
    }

    void eval() override
    {}
};

#endif