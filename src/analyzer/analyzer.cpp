#include "src/analyzer/analyzer.h"
#include "src/errors/error_messages.h"
#include <memory>
#include <string>
#include <exception>

Analyzer::Analyzer(Parser &p) : parser_(p){};

void Analyzer::analyze()
{
    try
    {
        std::unique_ptr<AST> tree = parser_.parse();
        tree->accept(*this);
    }
    catch(const std::runtime_error& e)
    {
        // Pass, analysis can't proceed, errors will be logged
    }
};

void Analyzer::logErrors(std::ostream &out)
{
    parser_.logErrors(out);
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