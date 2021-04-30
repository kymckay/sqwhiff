#pragma once

// Node types must be forward declared
class Compound;
class NoOp;
class NullaryOp;
class UnaryOp;
class BinaryOp;
class Assign;
class Variable;
class Number;
class StringLiteral;

class NodeVisitor
{
public:
    // Virtual destructor allows deletion of subclasses via base class pointer
    virtual ~NodeVisitor() {};

    // Declare overloads for each kind of a file to dispatch
    virtual void visit(Compound &) = 0;
    virtual void visit(NoOp &) = 0;
    virtual void visit(NullaryOp &) = 0;
    virtual void visit(UnaryOp &) = 0;
    virtual void visit(BinaryOp &) = 0;
    virtual void visit(Assign &) = 0;
    virtual void visit(Variable &) = 0;
    virtual void visit(Number &) = 0;
    virtual void visit(StringLiteral &) = 0;
};