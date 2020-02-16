#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "nodes.h"
#include "token.h"

namespace Parser
{
    RegexNode* parseRegex(std::vector<Token>::const_iterator& it);

    ProgramNode* buildTree(const std::vector<Token>& tokens);


}

#endif