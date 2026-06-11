#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "tokenizer.h"

class AstNode
{
    public:
        virtual ~AstNode()=default;
};



class ExprNode : public AstNode
{

};

class StmtNode : public AstNode
{

};

class DeclNode : public AstNode
{
    
};

class NumberExpr : public ExprNode
{
public:
    double value;

    explicit NumberExpr(double value)
        : value(value)
    {
    }
};

class VariableExpr : public ExprNode
{
public:
    Token identifier;

    explicit VariableExpr(Token identifier)
        : identifier(identifier)
    {
    }
};

class BinaryExpr : public ExprNode
{
public:
    TokenType op;

    std::unique_ptr<ExprNode> lhs;
    std::unique_ptr<ExprNode> rhs;

    BinaryExpr(
        TokenType op,
        std::unique_ptr<ExprNode> lhs,
        std::unique_ptr<ExprNode> rhs
    )
        : op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs))
    {
    }
};

class UnaryExpr : public ExprNode
{
public:
    TokenType op;

    std::unique_ptr<ExprNode> operand;

    UnaryExpr(
        TokenType op,
        std::unique_ptr<ExprNode> operand
    )
        : op(op),
          operand(std::move(operand))
    {
    }
};

class AssignmentExpr : public ExprNode
{
public:
    TokenType op;
    std::unique_ptr<ExprNode> lhs;
    std::unique_ptr<ExprNode> rhs;
    AssignmentExpr(
        TokenType op,
        std::unique_ptr<ExprNode> lhs,
        std::unique_ptr<ExprNode> rhs
    )
        : op(op),
          lhs(std::move(lhs)),
          rhs(std::move(rhs))
    {
    }
};

class CallExpr : public ExprNode
{
public:
    Token functionName;

    std::vector<std::unique_ptr<ExprNode>>
        arguments;

    CallExpr(
        Token functionName,
        std::vector<std::unique_ptr<ExprNode>> arguments
    )
        : functionName(functionName),
          arguments(std::move(arguments))
    {
    }
};

class DeclStmt : public StmtNode
{
public:
    std::unique_ptr<DeclNode>
        declaration;

    explicit DeclStmt(
        std::unique_ptr<DeclNode> declaration
    )
        : declaration(std::move(declaration))
    {
    }
};

class ExprStmt : public StmtNode
{
public:
    std::unique_ptr<ExprNode> expression;

    explicit ExprStmt(
        std::unique_ptr<ExprNode> expression
    )
        : expression(std::move(expression))
    {
    }
};

class ReturnStmt : public StmtNode
{
public:
    std::unique_ptr<ExprNode> value;

    explicit ReturnStmt(
        std::unique_ptr<ExprNode> value
    )
        : value(std::move(value))
    {
    }
};

class BlockStmt : public StmtNode
{
public:
    std::vector<
        std::unique_ptr<StmtNode>
    > statements;
};

class IfStmt : public StmtNode
{
public:
    std::unique_ptr<ExprNode>
        condition;

    std::unique_ptr<StmtNode>
        thenBranch;

    std::unique_ptr<StmtNode>
        elseBranch;
};

class WhileStmt : public StmtNode
{
public:
    std::unique_ptr<ExprNode>
        condition;

    std::unique_ptr<StmtNode>
        body;
};

class ForStmt : public StmtNode
{
public:
    std::unique_ptr<StmtNode>
        initializer;

    std::unique_ptr<ExprNode>
        condition;

    std::unique_ptr<ExprNode>
        increment;

    std::unique_ptr<StmtNode>
        body;
};

class ParameterDecl : public DeclNode
{
public:
    Token type;
    Token name;

    std::unique_ptr<ExprNode>
        defaultValue;
};

class VarDecl : public DeclNode
{
public:
    Token type;
    Token name;

    std::unique_ptr<ExprNode>
        initializer;
};

class FunctionDecl : public DeclNode
{
public:
    Token returnType;

    Token functionName;

    std::vector<
        std::unique_ptr<ParameterDecl>
    > parameters;

    std::unique_ptr<BlockStmt>
        body;
};
class Program : public AstNode
{
public:
    std::vector<
        std::unique_ptr<FunctionDecl>
    > functions;

    std::unique_ptr<BlockStmt>
        startBlock;
};
void printAST(
    const AstNode* node,
    int depth = 0
);