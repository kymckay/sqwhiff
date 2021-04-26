#pragma once
#include "node_visitor.h"
#include "parser.h"
#include "compound.h"
#include "no_op.h"
#include "unary_op.h"
#include "binary_op.h"
#include "assign.h"
#include "variable.h"
#include "number.h"

class Interpreter : NodeVisitor
{
    Parser &parser_;

public:
    Interpreter(Parser &);
    void interpret();

    // Visitor interface implementation

    void visit(Compound &) override;
    void visit(NoOp &) override;
    void visit(UnaryOp &) override;
    void visit(BinaryOp &) override;
    void visit(Assign &) override;
    void visit(Variable &) override;
    void visit(Number &) override;
};