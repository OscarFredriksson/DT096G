#include "parser.h"
#include "lexer.h"

//<expr> =  <op> | <op><expr>
//<op> = <ig-caps> | <or> | <str> | <group> | <count>
//<str> = <char>+
//<ig-caps> = <op> <"\I">
//<group> = <"("> <op> <")">
//<or> = <str> <"+"> <str>
//<count> = <char><"{N}"> | <group><"{N}"> | <dot><"{N}">
//<star> = <char><"*"> | <group><"*">
//<dot> = <".">
//<char> = <"letter"> | <dot> | <star> 
//<greedy> = <dot><star>


/*  lo* c.{3}

<expr>
    <op>
        <str>
            l
            <star>
                o

<expr>
    <op>
        <str>
            <l>
            <star>
                <o>
    <expr>
        <op>
            <str>
                < >
                <c>
                <count>
                    <dot>
*/

ExprNode* Parser::buildTree(Iter begin, Iter end)
{
    ExprNode* root = parseExpr(begin, end);

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

    DotNode* dotNode = new DotNode();

    begin++;

    return dotNode;
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
        
        CharNode* charNode = parseChar(begin, end);

        if(charNode)
        {
            strNode->addChild(charNode);
            continue;
        }

        DotNode* dotNode = parseDot(begin, end);

        if(!dotNode)    return nullptr;

        strNode->addChild(dotNode);
    }

    if(strNode->empty())    return nullptr;

    return strNode;
}

OpNode* Parser::parseOp(Iter& begin, Iter end)
{
    OpNode* opNode = new OpNode();

    GroupNode* groupNode = parseGroup(begin, end);

    if(groupNode)
    {
        opNode->addChild(groupNode);
        return opNode;
    }

    OrNode* orNode = parseOr(begin, end);

    if(orNode)
    {
        opNode->addChild(orNode);
        return opNode;
    }

    StrNode* strNode = parseStr(begin, end);

    if(strNode)    
    {
        opNode->addChild(strNode);
        return opNode;
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