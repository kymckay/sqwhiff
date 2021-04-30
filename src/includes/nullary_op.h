#pragma once
#include "ast.h"
#include "token.h"
#include "node_visitor.h"

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
