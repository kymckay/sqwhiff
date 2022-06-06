#pragma once
#include "sqwhiff/sqf/type.hpp"

namespace SQF {

struct UnaryCommand {
  Type right;
  Type value;

  UnaryCommand(Type r, Type v) : right(r), value(v){};
};

struct BinaryCommand {
  Type left;
  Type right;
  Type value;

  BinaryCommand(Type l, Type r, Type v) : left(l), right(r), value(v){};
};

}  // namespace SQF
