#pragma once
#include "src/ast/ast.h"
#include "src/ast/node_visitor.h"

class NoOp : public AST
{
public:
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};