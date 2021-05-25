#pragma once
#include "src/preprocessor/preprocessor.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

std::string preprocess(std::string s)
{
    std::stringstream ss(s);
    Preprocessor pp(ss);

    char c = pp.get();
    std::string result = "";
    while (c != '\0')
    {
        result.push_back(c);
        c = pp.get();
    }

    return result;
}