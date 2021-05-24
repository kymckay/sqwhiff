#pragma once
#include "src/preprocessor/preprocessor.h"
#include <string>

std::string test(Preprocessor &pp)
{
    std::string result = "";

    char c = pp.get();
    while (c != '\0')
    {
        result.push_back(c);
        c = pp.get();
    }

    return result;
}