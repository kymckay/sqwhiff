#pragma once
#include "node_visitor.h"
#include "parser.h"
#include "binary_op.h"
#include "number.h"

class Interpreter : NodeVisitor
{
    Parser &parser_;

public:
    Interpreter(Parser &);
    void interpret();

    // Visitor interface implementation

    void visit(BinaryOp &) override;
    void visit(Number &) override;
};