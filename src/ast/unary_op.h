#pragma once
#include "./ast.h"
#include "./node_visitor.h"
#include "../lexer/token.h"
#include <memory>

class UnaryOp : public AST
{
public:
    Token op;
    std::unique_ptr<AST> expr;

    UnaryOp(const Token &op, std::unique_ptr<AST> expr) : op(op), expr(std::move(expr)){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
