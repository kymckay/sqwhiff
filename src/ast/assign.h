#pragma once
#include "src/ast/ast.h"
#include "src/tokens/token.h"
#include "src/ast/node_visitor.h"
#include <memory>

class Assign : public AST
{
public:
    bool isPrivate;
    std::unique_ptr<AST> left;
    Token op;
    std::unique_ptr<AST> right;

    Assign(bool isPrivate, std::unique_ptr<AST> left, const Token &op, std::unique_ptr<AST> right) : isPrivate(isPrivate), left(std::move(left)), op(op), right(std::move(right)){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
