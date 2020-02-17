#include "parser.h"
#include "lexer.h"

ProgramNode* Parser::buildTree(std::vector<Token> tokens)
{
    std::vector<Token>::iterator token_it = tokens.begin();
    
    ProgramNode* root = new ProgramNode();

    Node* prev_node = root;

    while(token_it != tokens.end())
    {
        auto start_paren_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
        {
            return token_it.type == LEFT_PAREN;
        });

        if(start_paren_it == tokens.end())
        {
            Node* temp = parseExpression(tokens.begin(), tokens.end());

            prev_node->addChild(temp);

            prev_node = temp;
        }
        
        /*auto op_it = std::find_if(token_it, tokens.end(), [](const Token& token_it)
        {
            return token_it.type == OR;
        });

        if(op_it != std::end(tokens))
        {
            OrNode* or_node = new OrNode();
            
            //RegexNode* regexNode = parseRegex(token_it);
            
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

        token_it = std::next(op_it);*/
    }

    return root;
}

RegexNode* Parser::parseRegex(Iter& it)
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

Node* Parser::parseExpression(Iter begin, Iter end)
{
    RegexNode* root = new RegexNode();

    Node* prev_node = root;

    while(begin != end)
    {     
           
        Iter op_it = std::find_if(begin, end, [](const Token& token_it)
        {
            return token_it.type == OR;
        });

        if(op_it != end)
        {
            OrNode* or_node = new OrNode();
                        
            RegexNode* regexNode = new RegexNode();

            Node* prev_node_2 = regexNode;

            while(begin != op_it)
            {
                CharNode* charNode = new CharNode(begin->value);

                prev_node_2->addChild(charNode);

                prev_node_2 = charNode;

                begin++;
            }

            or_node->addChild(regexNode);

            prev_node->addChild(or_node);
            prev_node = or_node;
        }
        else    
        {
            RegexNode* regexNode = parseRegex(begin);

            prev_node->addChild(regexNode);
            break;
        }

        begin = std::next(op_it);

        begin->print();
        std::cout << "\n";
    }

    return root;
}