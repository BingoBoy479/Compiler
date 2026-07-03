#pragma once

#include "ast.h"
#include "tokenizer.h"

#include <vector>
#include <string>
#include <memory>
#include <stack>

class Parser
{
public:
    Parser(
        const std::vector<Token>& tokens,
        const std::string& source
    ): tokens(tokens),
      source(source)
    {

    };
    bool isAtEnd() const;
    const Token& consume(
        TokenType expected,
        const std::string& message
    );
    std::unique_ptr<ExprNode> parseExpression(bool stopAtRightParen = false);
    std::unique_ptr<StmtNode> parseStatement();

    std::unique_ptr<DeclNode> parseDeclaration();
    std::unique_ptr<Program> parseProgram();

private:
    const std::vector<Token>& tokens;
    const std::string& source;

    size_t current = 0;

    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();

    
    bool check(TokenType type) const;
    bool match(TokenType type);

    std::unique_ptr<VarDecl> parseVarDecl();
    std::unique_ptr<ForStmt> parseFor();
    std::unique_ptr<WhileStmt> parseWhile();
    std::unique_ptr<ExprNode> parseCallExpression();
    std::unique_ptr<StmtNode> parseIf();  


};