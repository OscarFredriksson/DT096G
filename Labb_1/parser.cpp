#include "parser.h"
#include "lexer.h"

//<prog> = <expr> | <expr><expr>

//<expr> =  <str> | <or> | <group> | <count> | <ig-caps> | <dot> | <star>
//<or> = <str> + <or> | <str> + <str>

//<group> = <or> | <str> | <count>

//<str> = <char> | <str><char>

//<dot> = <char>

//<star> = <char>

//<prog>
//  <expr>
//      <promise to>
//  <expr>
//      <group>
//          <or>
//              <love>
//              <hate>
//  <expr>
//      <you>

//<expr>
//  <expr>
//      <str>
//  <expr>
//      <group>
//          <dot-star>
//      <expr>
//          <str>


ExprNode* Parser::buildTree(std::vector<Token> tokens)
{
    std::vector<Token>::iterator token_it = tokens.begin();
    
    ExprNode* root = new ExprNode();

    Node* prev_node = root;

    for(;token_it <= tokens.end(); )
    {
        if(token_it->type == TokenType::CHAR)
        {
            Node* node = parseExpr(token_it, tokens.end());
            prev_node->addChild(node);
        }
        else if(token_it->type == TokenType::LEFT_PAREN)
        {
            Node* node = parseParen(token_it, tokens.end());
            prev_node->addChild(node);
        }
        else
        {
            token_it++;
        }

    }

    return root;
}

OrNode* Parser::parseOr(StrNode* leftChild, Iter& it, Iter end)
{
    OrNode* orNode = new OrNode();
    it++;

    StrNode* exprNode2 = parseStr(it, end);
    it++;

    orNode->addChild(leftChild);
    orNode->addChild(exprNode2);

    return orNode;
}

StrNode* Parser::parseStr(Iter& it, Iter end)
{
    StrNode* strNode = new StrNode();

    for(;it->type == TokenType::CHAR && it != end; it++)
    {
        CharNode* charNode = new CharNode(it->value);

        strNode->addChild(charNode);
    }

    return strNode;
}

Node* Parser::parseExpr(Iter& it, Iter end)
{
    ExprNode* root = new ExprNode();

    StrNode* node = parseStr(it, end);

    if(it->type == TokenType::OR)
    {
        OrNode* orNode = parseOr(node, it, end);

        root->addChild(orNode);
    }
    
    root->addChild(node);

    return root;
}

ParenNode* Parser::parseParen(Iter& it, Iter end)
{
    ParenNode* parenNode = new ParenNode();

    it++;

    Node* exprNode = parseExpr(it, end);

    parenNode->addChild(exprNode);

    return parenNode;
}

/*Node* Parser::parseStatement(Iter begin, Iter end)
{
    Iter token_it = begin;

    Node* root = nullptr;
    Node* prev_node;

    while(token_it != end)
    {
        auto op_it = std::find_if(token_it, end, [](const Token& token_it)
        {
            return token_it.type == OR;
        });

        if(op_it != end)
        {
            OrNode* or_node = new OrNode();

            or_node->addChild(parseExpression(token_it, op_it));

            if(!root)   root = or_node;
            else        prev_node->addChild(or_node);
            
            prev_node = or_node;

            token_it = std::next(op_it);
        }
        else    
        {
            ExprNode* exprNode = parseExpression(token_it, end);

            if(!root)   root = exprNode;
            else        prev_node->addChild(exprNode); 

            prev_node = exprNode;
        }
    }

    return root;
}*/