#pragma once
#include "ast.h"
#include "token.h"
#include "node_visitor.h"
#include <vector>
#include <memory>

// A compound node used to store "code" type (whole SQF file can be considered one big code type)
class Compound : public AST
{
public:
    // A compound AST node to capture an ordered list of statements
    std::vector<std::unique_ptr<AST>> children;

    Compound(std::vector<std::unique_ptr<AST>> children) : children(std::move(children)){};
    void accept(NodeVisitor &visitor) override
    {
        visitor.visit(*this);
    };
};