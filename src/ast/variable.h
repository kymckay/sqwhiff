#pragma once
#include "src/ast/ast.h"
#include "src/lexer/token.h"
#include "src/ast/node_visitor.h"

class Variable : public AST
{
public:
    Token token;

    Variable(const Token &t) : token(t){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
