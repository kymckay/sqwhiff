#pragma once

class BinaryOp;
class Number;

class NodeVisitor
{
public:
    // Declare overloads for each kind of a file to dispatch
    virtual void visit(BinaryOp &) = 0;
    virtual void visit(Number &) = 0;
};