#pragma once
#include <string>
#include <vector>

#include "sqwhiff/preprocessor/pos_char.hpp"

struct MacroDefinition {
  std::vector<std::string> params;
  std::string body;
};

struct MacroArg {
  int line = 1;
  int column = 1;
  std::string raw;
  PosStr chars;
};

typedef MacroDefinition MacroDefinition;
typedef MacroArg MacroArg;
