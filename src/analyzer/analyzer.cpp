#include "src/analyzer/analyzer.h"
#include "src/errors/error_messages.h"
#include <memory>
#include <string>
#include <ostream>

Analyzer::Analyzer(Parser &p) : parser_(p){};

void Analyzer::analyze(std::ostream &out)
{
    try
    {
        std::unique_ptr<AST> tree = parser_.parse();
        tree->accept(*this);
    }
    catch(const LexicalError& e)
    {
        out << e.what();
    }
    catch (const SyntaxError &e)
    {
        out << e.what();
    }

    // Log any encountered errors
    for (auto &&e : errors_)
    {
        out << e.token.line << ":" << e.token.column << " " << ErrorMessages.at(e.type);
    }
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