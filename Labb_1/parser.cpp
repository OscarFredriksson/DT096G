#include "parser.h"
#include "lexer.h"

RegexNode* Parser::parseRegex(std::vector<Token>::const_iterator& it)
{
    RegexNode* regexNode = new RegexNode();

    Node* prevNode = regexNode;

    for(;Lexer::isIdentifier(it->value); it++)
    {
        CharNode* charNode = new CharNode(it->value);

        prevNode->addChild(charNode);

        prevNode = charNode;
    }

    return regexNode;
}

ProgramNode* Parser::buildTree(const std::vector<Token>& tokens)
{
    std::vector<Token>::const_iterator token_it = tokens.begin();
    
    ProgramNode* root = new ProgramNode();

    Node* prev_node = root;

    while(token_it != tokens.end())
    {
        /*auto start_paren_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
        {
            return token_it.type == LEFT_PAREN;
        });

        if(start_paren_it == tokens.end())
        {
            //parsePlus();
        }*/
        
        
        


































        auto op_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
        {
            return token_it.type == OR;
        });

        if(op_it != std::end(tokens))
        {
            OrNode* or_node = new OrNode();
            
            RegexNode* regexNode = new RegexNode();

            Node* prev_node_2 = regexNode;

            while(token_it != op_it)
            {
                CharNode* charNode = new CharNode(token_it->value);

                prev_node_2->addChild(charNode);

                prev_node_2 = charNode;

                token_it++;
            }

            or_node->addChild(regexNode);

            prev_node->addChild(or_node);
            prev_node = or_node;
        }
        else    
        {
            RegexNode* regexNode = parseRegex(token_it);

            prev_node->addChild(regexNode);
            break;
        }

        token_it = std::next(op_it);
    }

    return root;
}