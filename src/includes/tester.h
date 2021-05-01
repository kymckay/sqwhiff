#pragma once
#include "node_visitor.h"
#include "parser.h"
#include "compound.h"
#include "no_op.h"
#include "nullary_op.h"
#include "unary_op.h"
#include "binary_op.h"
#include "assign.h"
#include "variable.h"
#include "code.h"
#include "number.h"
#include "string_literal.h"
#include <string>

class Tester : NodeVisitor
{
    Parser &parser_;
    std::string code_;

public:
    Tester(Parser &);
    void test();

    // Visitor interface implementation

    void visit(Compound &) override;
    void visit(NoOp &) override;
    void visit(NullaryOp &) override;
    void visit(UnaryOp &) override;
    void visit(BinaryOp &) override;
    void visit(Assign &) override;
    void visit(Variable &) override;
    void visit(Code &) override;
    void visit(Number &) override;
    void visit(StringLiteral &) override;
};