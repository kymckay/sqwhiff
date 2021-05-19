#pragma once
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
    std::vector<std::string> args;

    // Convenience conversion operator
    operator std::string() const { return word; }
};

typedef MacroDefinition MacroDefinition;
typedef MacroToken MacroToken;