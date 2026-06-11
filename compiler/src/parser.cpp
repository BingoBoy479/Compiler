#include "../include/parser.h"
#include <stdexcept>
#include <string_view>
#include <cassert>
#include <ostream>
#include <iostream>
using namespace std;
const Token& Parser::peek() const
{
    return tokens[current];
};

const Token& Parser::previous() const
{
    assert(current > 0);
    return tokens[current - 1];
};

const Token& Parser::advance()
{
    if(!isAtEnd())
        current++;

    return previous();
};

bool Parser::isAtEnd() const
{
    return peek().name == End;
};

bool Parser::check(TokenType type) const
{
    if(isAtEnd())
        return false;

    return peek().name == type;
};

bool Parser::match(TokenType type)
{
    if(!check(type))
        return false;

    advance();
    return true;
};

const Token& Parser::consume(
    TokenType expected,
    const std::string& message
)
{
    if(check(expected))
        return advance();

    throw std::runtime_error(message);
};

int precedence (const TokenType& type)
{
    switch(type)
    {
        case Assign:
        case PlusAssign:
        case MinusAssign:
            return 1;
        
        case OrOr:
            return 2;

        case AndAnd:
            return 3;
        
        case EqualEqual:
            return 4;
        
        case NotEqual:
            return 4;
        
        case Less:
        case LessEqual:
        case Greater:
        case GreaterEqual:
            return 5;
        
        case Plus:
        case Minus:
            return 6;
        
        case Star:
        case Slash:
        case Mod:
            return 7;
        
        case Not:
        case UnaryMinus:
        case Increase:
        case Decrease:
            return 8;

        case End:
            return -2;
        
        default:
            return -1;
    };
}

bool isRightAssociative(TokenType type)
{
    switch(type)
    {
        case Assign:
        case PlusAssign:
        case MinusAssign:
        case UnaryMinus:
        case Not:
        case Increase:
        case Decrease:
            return true;

        default:
            return false;
    }
}

bool isOperator(TokenType op)
{
    return precedence(op)>0;
}

void pushExpr(stack<unique_ptr<ExprNode>>& operands, unique_ptr<ExprNode> oper)
{
    operands.push(move(oper));
}

void pushOperator(stack<TokenType>& operators, TokenType op )
{
    operators.push(op);
}

unique_ptr<ExprNode> popExpr(stack<unique_ptr<ExprNode>>& operands)
{
    if(operands.empty())
        throw runtime_error("Expected operand");
    unique_ptr<ExprNode> res = move(operands.top());
    operands.pop();
    return res;
}

TokenType popOperator(stack<TokenType>& operators)
{
    if(operators.empty())
        throw runtime_error("Expected operand");
    TokenType res = operators.top();
    operators.pop();
    return res;
}

void collapse(stack<TokenType>& operators,stack<unique_ptr<ExprNode>>& operands)
{
    TokenType op = popOperator(operators);
    if  (op == UnaryMinus ||
         op == Not ||
         op == Increase ||
         op == Decrease
        )
    {
        unique_ptr<ExprNode> child = popExpr(operands);
        pushExpr(operands,make_unique<UnaryExpr>(op,move(child)));
        return;
    }
    else if(op==Assign || op == PlusAssign || op == MinusAssign)
    {
        unique_ptr<ExprNode> rhs = popExpr(operands);
         
        unique_ptr<ExprNode> lhs = popExpr(operands);
        pushExpr(operands,make_unique<AssignmentExpr>(op,move(lhs),move(rhs)));
        return;
    }
    else
    {
        unique_ptr<ExprNode> rhs = popExpr(operands);
         
        unique_ptr<ExprNode> lhs = popExpr(operands);
        pushExpr(operands,make_unique<BinaryExpr>(op,move(lhs),move(rhs)));
        return;
    }
}

unique_ptr<ExprNode>
Parser::parseCallExpression()
{
    // cerr << "Entering call: "
    //  << string(source.data()+peek().start,
    //            peek().length)
    //  << endl;
    Token functionName =
        consume(
            Word,
            "Expected function name"
        );

    consume(
        LeftParen,
        "Expected '(' after function name"
    );

    vector<
        unique_ptr<ExprNode>
    > arguments;

    if(!check(RightParen))
    {
        do
        {
            if(check(RightParen))
                break;
            arguments.push_back(
                parseExpression(true)
            );
        }
        while(match(Comma));
    }

    // cerr << "ARG COUNT = "
    //  << arguments.size()
    //  << endl;
    consume(
        RightParen,
        "Expected ')'"
    );

    return make_unique<CallExpr>(
        functionName,
        move(arguments)
    );
}

unique_ptr<ExprNode> Parser::parseExpression(bool stopAtRightParen)
{
    stack<unique_ptr<ExprNode>> operands ;
    stack<TokenType>            operators;

    bool expecting = true;
    while(!isAtEnd())
    {
        if(current>0)
        {
            const TokenType prev = previous().name;
            if(prev == Number || prev == RightParen || (prev == Word && peek().name!=LeftParen))
            {
                expecting = false;
            }
            else if(prev == LeftParen || prev == Comma || isOperator(prev) || (prev == Word && peek().name==LeftParen))
            {
                expecting = true;
            }
        }
        const Token& token = peek();

        if(token.name == Number)
        {
            double value = stod(string(source.data()+token.start,token.length));
            pushExpr(operands,make_unique<NumberExpr>(value));
            advance();
        }
        else if(token.name == Word)
        {
            if(current+1<tokens.size() && tokens[current+1].name == LeftParen)
            {
                pushExpr(operands,parseCallExpression());
                
            }
            else
            {
                pushExpr(operands,make_unique<VariableExpr>((token)));
                advance();
            }
        }
        else
        {
            TokenType op = token.name;

            if(op == Comma || op == Semicolon) 
            {
                break;
            }
            else if( op == LeftParen)
            {
                pushOperator(operators,op);
                advance();
            }
            else if(op == RightParen)
            {
                if(stopAtRightParen) break;
                while(!operators.empty() && operators.top()!=LeftParen)
                {
                    collapse(operators,operands);
                }
                if(operators.empty()) throw runtime_error("Invalid Parenthese");
                popOperator(operators);
                advance();
            }
            else if(isOperator(op))
            {
                if(op == Minus && expecting)
                {
                    op = UnaryMinus;
                }

                while(  (!operators.empty()) && 
                        (operators.top()!= LeftParen) &&
                        (
                            ( precedence(operators.top()) >  precedence(op) ) ||
                            ((precedence(operators.top()) == precedence(op)) && !isRightAssociative(op))
                        )
                )
                {
                    collapse(operators,operands);
                }
                pushOperator(operators,op);
                advance();
            }
            else
            {
                throw runtime_error("Unkown Token generated");
            }
        }

    }

    while(!operators.empty())
    {
        if(operators.top() == LeftParen)
            throw runtime_error("Invalid Parentheses");

        collapse(operators,operands);
    }
    
    if(operands.size() != 1)
    {
        throw runtime_error(
            "Malformed expression"
        );
    }
    
    auto res =  move(operands.top());
    operands.pop();
    return res;
};