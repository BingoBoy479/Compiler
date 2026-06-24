#include "../include/ast.h"
#include <iostream>

using namespace std;

static void indent(int depth)
{
    for(int i = 0; i < depth; i++)
        cout << "  ";
}

void printAST(const AstNode* node, int depth)
{
    if(node == nullptr)
    {
        indent(depth);
        cout << "null\n";
        return;
    }

    if(auto n = dynamic_cast<const NumberExpr*>(node))
    {
        indent(depth);
        cout << "Number(" << n->value << ")\n";
    }
    else if(dynamic_cast<const VariableExpr*>(node))
    {
        indent(depth);
        cout << "Variable\n";
    }
    else if(auto b = dynamic_cast<const BinaryExpr*>(node))
    {
        indent(depth);
        cout << "Binary(" << b->op << ")\n";

        printAST(b->lhs.get(), depth + 1);
        printAST(b->rhs.get(), depth + 1);
    }
    else if(auto u = dynamic_cast<const UnaryExpr*>(node))
    {
        indent(depth);
        cout << "Unary(" << u->op << ")\n";

        printAST(u->operand.get(), depth + 1);
    }
    else if(auto c = dynamic_cast<const CallExpr*>(node))
    {
        indent(depth);
        cout << "Call\n";

        for(const auto& arg : c->arguments)
        {
            printAST(arg.get(), depth + 1);
        }
    }
    else if(auto a = dynamic_cast<const AssignmentExpr*>(node))
    {
        indent(depth);
        cout << "Assignment\n";

        printAST(a->lhs.get(), depth + 1);
        printAST(a->rhs.get(), depth + 1);
    }
    else if(auto e = dynamic_cast<const ExprStmt*>(node))
    {
        indent(depth);
        cout << "ExprStmt\n";

        printAST(
            e->expression.get(),
            depth + 1
        );
    }
    else if(auto r = dynamic_cast<const ReturnStmt*>(node))
    {
        indent(depth);
        cout << "ReturnStmt\n";

        printAST(
            r->value.get(),
            depth + 1
        );
    }
    else if(auto b = dynamic_cast<const BlockStmt*>(node))
    {
        indent(depth);
        cout << "BlockStmt\n";

        for(const auto& stmt : b->statements)
        {
            printAST(
                stmt.get(),
                depth + 1
            );
        }
    }
    else if(auto i = dynamic_cast<const IfStmt*>(node))
    {
        indent(depth);
        cout << "IfStmt\n";

        indent(depth + 1);
        cout << "Condition\n";

        printAST(
            i->condition.get(),
            depth + 2
        );

        indent(depth + 1);
        cout << "Then\n";

        printAST(
            i->thenBranch.get(),
            depth + 2
        );

        if(i->elseBranch)
        {
            indent(depth + 1);
            cout << "Else\n";

            printAST(
                i->elseBranch.get(),
                depth + 2
            );
        }
    }
    else
    {
        indent(depth);
        cout << "Unknown Node\n";
    }
}