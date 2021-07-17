#pragma once
#include "./ast.h"
#include "./node_visitor.h"
#include "../lexer/token.h"

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
