#pragma once
#include "ast.h"
#include "token.h"
#include "node_visitor.h"

class StringLiteral : public AST
{
public:
    Token token;

    StringLiteral(const Token &t) : token(t){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
