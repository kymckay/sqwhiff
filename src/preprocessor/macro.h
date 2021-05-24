#pragma once
#include "src/preprocessor/pos_char.h"
#include <string>
#include <vector>

struct MacroDefinition
{
    std::vector<std::string> params;
    std::vector<std::string> body;
};

struct MacroToken
{
    int line = 1;
    int column = 1;
    std::string word;
    std::string raw_args;
    std::vector<std::string> args;
    std::vector<PosChar> expanded;
};

typedef MacroDefinition MacroDefinition;
typedef MacroToken MacroToken;