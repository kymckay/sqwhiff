#pragma once
#include "./ast.h"
#include "./node_visitor.h"
#include "../lexer/token.h"
#include <memory>

class BinaryOp : public AST
{
public:
    std::unique_ptr<AST> left;
    Token op;
    std::unique_ptr<AST> right;

    BinaryOp(std::unique_ptr<AST> left, const Token &op, std::unique_ptr<AST> right) : left(std::move(left)), op(op), right(std::move(right)){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
