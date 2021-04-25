#pragma once

// Node types must be forward declared
class BinaryOp;
class Number;

class NodeVisitor
{
public:
    // Virtual destructor allows deletion of subclasses via base class pointer
    virtual ~NodeVisitor() {};

    // Declare overloads for each kind of a file to dispatch
    virtual void visit(BinaryOp &) = 0;
    virtual void visit(Number &) = 0;
};