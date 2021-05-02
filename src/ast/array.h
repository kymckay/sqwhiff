#pragma once
#include "src/ast/ast.h"
#include "src/tokens/token.h"
#include "src/ast/node_visitor.h"
#include <vector>
#include <memory>

// An array display contains a compound node (or NoOp node) where each child is an expression (returns something)
class Array : public AST
{
public:
    // A compound AST node to capture an ordered list of statements (in this case expressions)
    std::unique_ptr<AST> expressions;

    Array(std::unique_ptr<AST> expressions) : expressions(std::move(expressions)){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};
