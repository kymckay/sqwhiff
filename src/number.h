#pragma once
#include "ast.h"
#include "token.h"
#include "node_visitor.h"

class Number : public AST
{
    Token token_;

public:
    Number(const Token &t) : token_(t) {};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};

