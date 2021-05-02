#pragma once
#include "src/parser/parser.h"
#include "src/ast/node_visitor.h"
#include "src/ast/compound.h"
#include "src/ast/no_op.h"
#include "src/ast/nullary_op.h"
#include "src/ast/unary_op.h"
#include "src/ast/binary_op.h"
#include "src/ast/assign.h"
#include "src/ast/variable.h"
#include "src/ast/array.h"
#include "src/ast/code.h"
#include "src/ast/number.h"
#include "src/ast/string_literal.h"
#include <string>

class Tester : NodeVisitor
{
    Parser &parser_;
    std::string code_;

public:
    Tester(Parser &);
    std::string test();

    // Visitor interface implementation

    void visit(Compound &) override;
    void visit(NoOp &) override;
    void visit(NullaryOp &) override;
    void visit(UnaryOp &) override;
    void visit(BinaryOp &) override;
    void visit(Assign &) override;
    void visit(Variable &) override;
    void visit(Array &) override;
    void visit(Code &) override;
    void visit(Number &) override;
    void visit(StringLiteral &) override;
};