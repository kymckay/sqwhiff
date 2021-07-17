#include "src/analyzer/analyzer.h"
#include "src/sqf/all_keywords.h"
#include <memory>
#include <string>
#include <ostream>

Analyzer::Analyzer(Parser &p, std::ostream &out) : parser_(p), out_(out){};

int Analyzer::analyze()
{
    try
    {
        std::unique_ptr<AST> tree = parser_.parse();
        tree->accept(*this);
    }
    catch (const PreprocessingError &e)
    {
        out_ << e.what();
        return 1;
    }
    catch(const LexicalError &e)
    {
        out_ << e.what();
        return 1;
    }
    catch (const SyntaxError &e)
    {
        out_ << e.what();
        return 1;
    }

    return errorc_;
};

void Analyzer::error(Token t, std::string msg) {
    out_ << t.line << ':' << t.column << " SemanticError - " << msg << ": " << t.raw << '\n';
    errorc_++;
}

void Analyzer::visit(Compound &c)
{
    for (auto &&child : c.children)
    {
        child->accept(*this);
    }
};

void Analyzer::visit(NoOp &){
};

void Analyzer::visit(NullaryOp &op)
{
};

void Analyzer::visit(UnaryOp &op)
{
    if (SQF_Unary_Keywords.find(op.op.raw) == SQF_Unary_Keywords.end())
    {
        error(op.op, "Incorrect arity, expected binary use");
    }

    op.expr->accept(*this);
};

void Analyzer::visit(BinaryOp &op)
{
    if (SQF_Binary_Keywords.find(op.op.raw) == SQF_Binary_Keywords.end())
    {
        error(op.op, "Incorrect arity, expected unary use");
    }

    op.left->accept(*this);
    op.right->accept(*this);
};

void Analyzer::visit(Assign &op)
{
    op.right->accept(*this);
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