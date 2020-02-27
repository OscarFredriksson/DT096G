#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "nodes.h"
#include "token.h"

//<expr> =  <op> | <op><expr>
//<op> = <ig-caps> | <group> | <or> | <str> |  <count>
//<str> = <star> | (<char> | <dot>)+
//<ig-caps> = <op> <"\I">
//<group> = <"("> <op> <")">
//<or> = <str> <"+"> <str>
//<count> = <char><"{N}"> | <group><"{N}"> | <dot><"{N}">
//<star> = <char><"*"> | <group><"*">
//<dot> = <".">
//<char> = <"letter"> | <dot> | <star> 
//<greedy> = <dot><star>

using Iter = std::vector<Token>::iterator;

namespace Parser
{
    ExprNode* buildTree(Iter begin, Iter end);

    CharNode* parseChar(Iter& begin, Iter end);

    DotNode* parseDot(Iter& begin, Iter end);

    StarNode* parseStar(Iter& begin, Iter end);

    StrNode* parseStr(Iter& begin, Iter end);

    OrNode* parseOr(Iter& it, Iter end);

    OpNode* parseOp(Iter& begin, Iter end);

    GroupNode* parseGroup(Iter& begin, Iter end);

    ExprNode* parseExpr(Iter& begin, Iter end);
}

#endif