#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "nodes.h"
#include "token.h"

using Iter = std::vector<Token>::iterator;

namespace Parser
{
    StrNode* parseStr(Iter& it, Iter end);

    OrNode* parseOr(StrNode* leftChild, Iter& it, Iter end);

    ExprNode* buildTree(std::vector<Token> tokens);

    ASTNode* parseOp(Iter& it, Iter end);

    ParenNode* parseParen(Iter& it, Iter end);
}

#endif