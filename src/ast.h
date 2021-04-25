#pragma once
#include "node_visitor.h"

class AST
{
public:
    virtual void accept(NodeVisitor &) = 0;
};