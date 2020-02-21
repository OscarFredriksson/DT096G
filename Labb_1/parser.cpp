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
            <l>
            <str>
                <star>
                    <o>

    <expr>
        <op>
            <str>
                < >
                <str>
                    <c>
                    <str>
                        <count>
                            <dot>xs

    <str>
        <l>
        <star>
            <o>
*/
ExprNode* Parser::buildTree(std::vector<Token> tokens)
{
    std::vector<Token>::iterator token_it = tokens.begin();
    
    ExprNode* root = new ExprNode();

    ASTNode* prev_node = root;

    for(;token_it != tokens.end();)
    {
        if(token_it->type == TokenType::CHAR)
        {
            ASTNode* node = parseOp(token_it, tokens.end());
            prev_node->addChild(node);
        }
        else if(token_it->type == TokenType::LEFT_PAREN)
        {
            ASTNode* node = parseParen(token_it, tokens.end());
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

    StrNode* rightChild = parseStr(it, end);

    orNode->addChild(leftChild);
    orNode->addChild(rightChild);

    return orNode;
}

StrNode* Parser::parseStr(Iter& it, Iter end)
{
    StrNode* strNode = new StrNode();

    while(it->type == TokenType::CHAR && it != end)
    {
        CharNode* charNode = new CharNode(it->value);

        strNode->addChild(charNode);
        it++;
    }

    return strNode;
}

ASTNode* Parser::parseOp(Iter& it, Iter end)
{
    OpNode* root = new OpNode();

    StrNode* node = parseStr(it, end);

    if(it == end)
    {
        root->addChild(node);
    }
    else if(it->type == TokenType::OR)
    {
        OrNode* orNode = parseOr(node, it, end);

        root->addChild(orNode);
    }
        
    return root;
}

ParenNode* Parser::parseParen(Iter& it, Iter end)
{ 
    std::cout << "parse paren\n";

    ParenNode* parenNode = new ParenNode();

    it++;

    ASTNode* exprNode = parseOp(it, end);

    parenNode->addChild(exprNode);

    return parenNode;
}