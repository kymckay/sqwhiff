#include "interpreter.h"
#include "ast.h"
#include <memory>
#include <iostream>

Interpreter::Interpreter(Parser &p) : parser_(p) {};

void Interpreter::interpret(){
    std::unique_ptr<AST> tree = parser_.parse();
    tree->accept(*this);
};

void Interpreter::visit(BinaryOp &op)
{
    // TODO: recursive visitor accept calls
    std::cout << "binary operator";
};

void Interpreter::visit(UnaryOp &op)
{
    // TODO: recursive visitor accept calls
    std::cout << "unary operator";
};

void Interpreter::visit(Number &num)
{
    std::cout << "number";
};