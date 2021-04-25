#include "interpreter.h"

Interpreter::Interpreter(Parser &p) : parser_(p) {};

void Interpreter::visit(BinaryOp &)
{

};

void Interpreter::visit(Number &)
{
    
};