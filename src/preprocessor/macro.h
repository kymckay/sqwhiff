#pragma once
#include "src/preprocessor/pos_char.h"
#include <string>
#include <vector>

struct MacroDefinition
{
    std::vector<std::string> params;
    std::string body;
};

struct MacroArg
{
    int line = 1;
    int column = 1;
    std::string raw;
    std::vector<PosChar> chars;
};

typedef MacroDefinition MacroDefinition;
typedef MacroArg MacroArg;