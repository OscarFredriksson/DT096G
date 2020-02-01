#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string>

enum Operator
{
    plus,
    minus, 
    div,
    mult
};

Operator getOp(char c)
{
    switch(c)
    {
        case '+':   return Operator::plus;
        case '-':   return Operator::minus;
        case '*':   return Operator::mult;
        case '/':   return Operator::div;
        default:    throw std::runtime_error("Error: Undefined operator");
    }
}

int calc(int first, int second, Operator op)
{
    switch(op)
    {
        case Operator::plus:    return first + second;
        case Operator::minus:   return first - second;
        case Operator::div:     return first / second;
        case Operator::mult:    return first * second;
    }
}

int charToInt(char c)
{
    return c - '0';
}

int parseInt(std::string::iterator& it)
{
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

    while(std::cin >> line)
    {
        if(line == "exit") return 0;

        Operator op;

        std::string::iterator it = line.begin();

        int sum;
        
        try
        {
            sum = parseInt(it);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }

        try
        {
            op = getOp(*it);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        it++;

        try
        {
            int val = parseInt(it); 
            sum = calc(sum, val, op);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }  

        std::cout << sum << "\n";
    }

    return 0;
}