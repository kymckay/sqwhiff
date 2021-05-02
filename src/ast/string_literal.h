#pragma once
#include "src/ast/ast.h"
#include "src/tokens/token.h"
#include "src/ast/node_visitor.h"

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
