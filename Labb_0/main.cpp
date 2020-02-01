#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <vector>

#include "tokens.h"

Operator getOp(char c)
{
    switch(c)
    {
        case '+':   return Operator::Plus;
        case '-':   return Operator::Minus;
        case '*':   return Operator::Mult;
        case '/':   return Operator::Div;
        default:    throw std::runtime_error("Error: Undefined operator");
    }
}

int calc(int first, int second, Operator op)
{
    switch(op)
    {
        case Operator::Plus:    return first + second;
        case Operator::Minus:   return first - second;
        case Operator::Div:     return first / second;
        case Operator::Mult:    return first * second;
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

Operator parseOperator(std::string::iterator& it)
{
    trimLeadingWhitespace(it);
    Operator op = getOp(*it);
    it++;
    return op;
}

void old_parse()
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
}

std::vector<Token*> tokenize(std::string line)
{
    std::vector<Token*> tokens;

    std::string::iterator it = line.begin();

    for(int i = 0; it != line.end(); i++)
    {
        trimLeadingWhitespace(it);

        if(isdigit(*it))    
        {
            try
            {                
                int val = parseInt(it);
                tokens.push_back(new IntegerToken(val));
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
                Operator val = parseOperator(it);
                tokens.push_back(new OperatorToken(val));
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }    
        }
    }

    return tokens;
}

int main(int argc, char* argv[])
{
    //parse();

    std::string line;
    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

        std::vector<Token*> tokens = tokenize(line);
        
       for(auto token: tokens)  token->print();
       std::cout << "\n";
    }

    return 0;
}