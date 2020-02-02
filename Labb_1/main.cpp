#include <iostream>
#include <string>
#include <vector>

#include "token.h"
#include "parser.h"

int main()
{
    std::string line;
    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

        Parser parser(line);

        std::vector<Token> tokens = parser.tokenize();
        
       for(auto token: tokens)  token.print();
       std::cout << "\n";
    }
 
    return 0;
}