#pragma once
#include "./ast.h"
#include "./node_visitor.h"

class NoOp : public AST
{
public:
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
