#include "src/analyzer/analyzer.h"
#include <memory>
#include <string>

Analyzer::Analyzer(Parser &p) : parser_(p){};

void Analyzer::analyze()
{
    std::unique_ptr<AST> tree = parser_.parse();
    tree->accept(*this);
};

void Analyzer::visit(Compound &c)
{
};

void Analyzer::visit(NoOp &){
};

void Analyzer::visit(NullaryOp &op)
{
};

void Analyzer::visit(UnaryOp &op)
{
};

void Analyzer::visit(BinaryOp &op)
{
};

void Analyzer::visit(Assign &op)
{
};

void Analyzer::visit(Variable &var)
{
};

void Analyzer::visit(Array &node)
{
};

void Analyzer::visit(Code &node)
{
};

void Analyzer::visit(Number &num)
{
};

void Analyzer::visit(StringLiteral &s)
{
};