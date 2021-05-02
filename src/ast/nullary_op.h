#pragma once
#include "src/ast/ast.h"
#include "src/tokens/token.h"
#include "src/ast/node_visitor.h"

class NullaryOp : public AST
{
public:
    Token op;

    NullaryOp(const Token &op) : op(op) {};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
