#include "parser.h"
#include "lexer.h"

ProgramNode* Parser::buildTree(std::vector<Token> tokens)
{
    std::vector<Token>::iterator token_it = tokens.begin();
    
    ProgramNode* root = new ProgramNode();

    Node* prev_node = root;

    auto start_paren_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
    {
        return token_it.type == LEFT_PAREN;
    });

    auto end_paren_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
    {
        return token_it.type == RIGHT_PAREN;
    });

    if(start_paren_it == tokens.end())
    {
        Node* temp = parseExpression(tokens.begin(), tokens.end());

        prev_node->addChild(temp);
    }
    else if(end_paren_it == tokens.end())
    {
        std::cerr << "Found no closing parentheses\n";
    }
    else
    {
        Node* parenNode = new ParenNode();

        prev_node->addChild(parenNode);

        prev_node = parenNode;

        parenNode->addChild(parseExpression(token_it, start_paren_it));

        Node* innerExp = parseExpression(std::next(start_paren_it), end_paren_it);

        parenNode->addChild(innerExp);

        parenNode->addChild(parseExpression(std::next(end_paren_it), tokens.end()));

        prev_node = parenNode;
    }

    return root;
}

RegexNode* Parser::parseRegex(Iter& it, Iter end)
{
    RegexNode* regexNode = new RegexNode();

    Node* prevNode = regexNode;

    for(;Lexer::isIdentifier(it->value) && it != end; it++)
    {
        CharNode* charNode = new CharNode(it->value);

        prevNode->addChild(charNode);

        prevNode = charNode;
    }

    return regexNode;
}

Node* Parser::parseExpression(Iter begin, Iter end)
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

            or_node->addChild(parseRegex(token_it, op_it));

            if(!root)   root = or_node;
            else        prev_node->addChild(or_node);
            
            prev_node = or_node;

            token_it = std::next(op_it);
        }
        else    
        {
            RegexNode* regexNode = parseRegex(token_it, end);

            if(!root)   root = regexNode;
            else        prev_node->addChild(regexNode); 

            prev_node = regexNode;
        }
    }

    return root;
}