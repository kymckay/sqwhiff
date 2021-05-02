#pragma once
#include "src/ast/ast.h"
#include "src/tokens/token.h"
#include "src/ast/node_visitor.h"
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
