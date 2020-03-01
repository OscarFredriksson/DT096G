#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "nodes.h"
#include "token.h"

/* Grammar:

    <prgm> = <expr>
    <expr> =  <op> | <op><expr>
    <op> = <ig-caps> | <sub-op>
    <sub-op> = <group> | <greedy> | <or> | <str>
    <str> = <star> | <count> | (<dot> | <char>)+
    <ig-caps> = <sub-op> <"\I">
    <group> = <"("> <expr> <")">
    <or> = <str> <"+"> <str>
    <count> = <char><"{N}"> | <dot><"{N}">
    <star> = <char><"*">
    <dot> = <".">
    <char> = <"letter">
    <greedy> = <dot><star>
*/

using Iter = std::vector<Token>::iterator;

namespace Parser
{
    PrgmNode* buildTree(Iter begin, Iter end);

    CharNode* parseChar(Iter& begin, Iter end);

    IgCapsNode* parseIgCaps(Iter& begin, Iter end);

    CountNode* parseCount(Iter& begin, Iter end);

    DotNode* parseDot(Iter& begin, Iter end);

    StarNode* parseStar(Iter& begin, Iter end);

    GreedyNode* parseGreedy(Iter& begin, Iter end);

    StrNode* parseStr(Iter& begin, Iter end);

    OrNode* parseOr(Iter& it, Iter end);

    OpNode* parseOp(Iter& begin, Iter end);

    SubOpNode* parseSubOp(Iter& begin, Iter end);

    GroupNode* parseGroup(Iter& begin, Iter end);

    ExprNode* parseExpr(Iter& begin, Iter end);
}

#endif