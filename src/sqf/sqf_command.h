#pragma once
#include "src/sqf/sqf_type.h"

struct SQFUnaryCommand
{
    SQFType right;
    SQFType value;

    SQFUnaryCommand(SQFType r, SQFType v) : right(r), value(v){};
};

struct SQFBinaryCommand
{
    SQFType left;
    SQFType right;
    SQFType value;

    SQFBinaryCommand(SQFType l, SQFType r, SQFType v) : left(l), right(r), value(v){};
};

// Using a typedef prevents naming clashes in global name space
typedef SQFUnaryCommand SQFUnaryCommand;
typedef SQFBinaryCommand SQFBinaryCommand;