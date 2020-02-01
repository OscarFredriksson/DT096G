#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <algorithm>

enum Operator
{
    plus,
    minus, 
    divi,
    mult
};

Operator getOp(char c)
{
    switch(c)
    {
        case '+':   return Operator::plus;
        case '-':   return Operator::minus;
        case '*':   return Operator::mult;
        case '/':   return Operator::divi;
        default:    throw std::runtime_error("Error: Undefined operator");
    }
}

int calc(int first, int second, Operator op)
{
    switch(op)
    {
        case Operator::plus:    return first + second;
        case Operator::minus:   return first - second;
        case Operator::divi:     return first / second;
        case Operator::mult:    return first * second;
    }
}

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

int main(int argc, char* argv[])
{
    std::string line;

    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

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
                    trimLeadingWhitespace(it);
                    op = getOp(*it);
                    it++;
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

    return 0;
}