#include "parser.h"
#include "lexer.h"

/* Grammar:

    <prgm> = <expr>
    <expr> =  <op> | <op><expr>
    <op> = <ig-caps> | <sub-op>
    <sub-op> = <group> | <greedy> | <or> | <str>
    <str> = <star> | <count> | (<dot> | <char>)+
    <ig-caps> = <sub-op> <"\I">
    <group> = <"("> <op> <")">
    <or> = <str> <"+"> <str>
    <count> = <char><"{N}"> | <dot><"{N}">
    <star> = <char><"*">
    <dot> = <".">
    <char> = <"letter">
    <greedy> = <dot><star>
*/

PrgmNode* Parser::buildTree(Iter begin, Iter end)
{
    PrgmNode* root = new PrgmNode();

    ExprNode* exprNode = parseExpr(begin, end);

    if(!exprNode)   return nullptr;

    root->addChild(exprNode);

    return root;
}

OrNode* Parser::parseOr(Iter& begin, Iter end)
{
    Iter prev_begin = begin;

    StrNode* leftStr = parseStr(begin, end);

    if(!leftStr)    return nullptr;

    if(begin->type != TokenType::OR || begin == end) 
    {
        begin = prev_begin;
        return nullptr;
    }

    begin++;
    
    OrNode* orNode = new OrNode();

    StrNode* rightStr = parseStr(begin, end);

    if(!rightStr)   return nullptr;

    orNode->addChild(leftStr);
    orNode->addChild(rightStr);

    return orNode;
}

CharNode* Parser::parseChar(Iter& begin, Iter end)
{
    if(begin->type != TokenType::CHAR) return nullptr;

    CharNode* charNode = new CharNode(begin->value);

    begin++;

    return charNode;
}

DotNode* Parser::parseDot(Iter& begin, Iter end)
{
    if(begin->type != TokenType::DOT) return nullptr;

    Iter prev_begin = begin++;

    if(begin->type == TokenType::STAR)
    {
        begin = prev_begin;
        return nullptr;
    }

    DotNode* dotNode = new DotNode();

    return dotNode;
}

CountNode* Parser::parseCount(Iter& begin, Iter end)
{
    Iter prev_begin = begin;

    ASTNode* node = parseDot(begin, end);

    if(!node)
    {
        node = parseChar(begin, end);

        if(!node) return nullptr;
    }

    if(begin->type != TokenType::COUNTER || begin == end) 
    {
        begin = prev_begin;
        return nullptr;
    }

    CountNode* countNode = new CountNode(begin->value - '0');   //-'0' convert to int

    countNode->addChild(node);

    begin++;

    return countNode;
}

StarNode* Parser::parseStar(Iter& begin, Iter end)
{
    if(begin->type != TokenType::CHAR) return nullptr;
    
    CharNode* charNode = new CharNode(begin->value);

    begin++;

    if(begin->type != TokenType::STAR)  return nullptr;

    begin++;

    StarNode* starNode = new StarNode();

    starNode->addChild(charNode);

    return starNode;
}

GreedyNode* Parser::parseGreedy(Iter& begin, Iter end)
{
    GreedyNode* greedyNode = new GreedyNode();

    if(begin->type != TokenType::DOT || begin == end) return nullptr;

    begin++;

    if(begin->type != TokenType::STAR || begin == end) return nullptr;

    begin++;

    return greedyNode;
}

StrNode* Parser::parseStr(Iter& begin, Iter end)
{
    StrNode* strNode = new StrNode();

    while(( begin->type == TokenType::CHAR || 
            begin->type == TokenType::DOT) &&
            begin != end)
    {
        Iter prev_begin = begin;

        StarNode* starNode = parseStar(begin, end);

        if(starNode)
        {
            strNode->addChild(starNode);
            break;  //Star-node är terminal
        }
        
        begin = prev_begin;

        CountNode* countNode = parseCount(begin, end);

        if(countNode)
        {
            strNode->addChild(countNode);
            break;  //Count-node är terminal
        }
        
        CharNode* charNode = parseChar(begin, end);

        if(charNode)
        {
            strNode->addChild(charNode);
            continue;
        }

        DotNode* dotNode = parseDot(begin, end);

        if(!dotNode)    break;

        strNode->addChild(dotNode);
    }

    if(strNode->empty())    return nullptr;

    return strNode;
}

IgCapsNode* Parser::parseIgCaps(Iter& begin, Iter end)
{
    IgCapsNode* igCapsNode = new IgCapsNode();

    Iter prev_begin = begin;

    SubOpNode* subOpNode = parseSubOp(begin, end);

    if(!subOpNode)
    {
        begin = prev_begin;
        return nullptr;
    }

    if(begin->type != TokenType::IGNORE_CAPS || begin == end)
    {
        begin = prev_begin;
        return nullptr;
    }

    begin++;

    igCapsNode->addChild(subOpNode);

    return igCapsNode;
}

OpNode* Parser::parseOp(Iter& begin, Iter end)
{
    OpNode* opNode = new OpNode();

    if(begin == end) return nullptr;

    IgCapsNode* igCapsNode = parseIgCaps(begin, end);

    if(igCapsNode)
    {
        opNode->addChild(igCapsNode);
        return opNode;
    }

    SubOpNode* subOpNode = parseSubOp(begin, end);

    if(!subOpNode)  return nullptr;

    opNode->addChild(subOpNode);

    return opNode;
}

SubOpNode* Parser::parseSubOp(Iter& begin, Iter end)
{
    SubOpNode* subOpNode = new SubOpNode();

    GroupNode* groupNode = parseGroup(begin, end);

    if(groupNode)
    {
        subOpNode->addChild(groupNode);
        return subOpNode;
    }

    Iter prev_begin = begin;

    GreedyNode* greedyNode = parseGreedy(begin, end);

    if(greedyNode)
    {
        subOpNode->addChild(greedyNode);
        return subOpNode;
    }

    OrNode* orNode = parseOr(begin, end);

    if(orNode)
    {
        subOpNode->addChild(orNode);
        return subOpNode;
    }

    begin = prev_begin;

    StrNode* strNode = parseStr(begin, end);

    if(strNode)    
    {
        subOpNode->addChild(strNode);
        return subOpNode;
    }

    return nullptr;
}

GroupNode* Parser::parseGroup(Iter& begin, Iter end)
{ 
    GroupNode* groupNode = new GroupNode();

    if(begin->type != TokenType::LEFT_PAREN && begin != end) 
    {
        return nullptr;
    }

    OpNode* opNode = parseOp(++begin, end);

    if(!opNode || begin->type != TokenType::RIGHT_PAREN) return nullptr;
    
    groupNode->addChild(opNode);

    if(begin != end) begin++;

    return groupNode;
}

ExprNode* Parser::parseExpr(Iter& begin, Iter end)
{
    if(begin == end) return nullptr;

    ExprNode* exprNode = new ExprNode();

    OpNode* opNode = parseOp(begin, end);

    if(!opNode) return nullptr;

    exprNode->addChild(opNode);

    if(begin == end)    return exprNode;

    ExprNode* exprNode2 = parseExpr(begin, end);

    if(exprNode2)      exprNode->addChild(exprNode2);

    return exprNode;
}