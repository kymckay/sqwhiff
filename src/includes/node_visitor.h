#pragma once

// Node types must be forward declared
class Compound;
class NoOp;
class UnaryOp;
class BinaryOp;
class Assign;
class Variable;
class Number;

class NodeVisitor
{
public:
    // Virtual destructor allows deletion of subclasses via base class pointer
    virtual ~NodeVisitor() {};

    // Declare overloads for each kind of a file to dispatch
    virtual void visit(Compound &) = 0;
    virtual void visit(NoOp &) = 0;
    virtual void visit(UnaryOp &) = 0;
    virtual void visit(BinaryOp &) = 0;
    virtual void visit(Assign &) = 0;
    virtual void visit(Variable &) = 0;
    virtual void visit(Number &) = 0;
};