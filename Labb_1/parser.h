#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "nodes.h"
#include "token.h"

using Iter = std::vector<Token>::iterator;

namespace Parser
{
    RegexNode* parseRegex(Iter& it, Iter end);

    ProgramNode* buildTree(std::vector<Token> tokens);

    Node* parseExpression(Iter begin, Iter end);
}

#endif