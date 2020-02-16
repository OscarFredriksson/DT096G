#include <iostream>
#include <string>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "nodes.h"
#include "parser.h"

int main()
{   
    std::string txt = "Waterloo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";

    std::string line;

    while(std::getline(std::cin, line))
    {
        if(line == "exit") return 0;

        Lexer lexer(line);

        std::vector<Token> tokens = lexer.tokenize();
        
        std::cout << "\nTokens: ";
        for(auto token: tokens)  token.print();
        std::cout << "\n\n";

        ProgramNode* root = Parser::buildTree(tokens);

        root->print();

        std::cout << "\n" << root->Node::eval(txt.begin(), txt.end()) << "\n";
    }
 
    return 0;
}