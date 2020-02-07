#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "nodes.h"
#include "token.h"

class Parser
{
public:
    /*Parser::Parser(const std::vector<Token>& tokens):
          tokens(tokens)
    {}*/

    RegexNode* parseRegex(std::vector<Token>::const_iterator& it);

    ProgramNode* buildTree(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;

};

#endif