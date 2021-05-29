#pragma once
#include "src/preprocessor/preprocessor.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

class PreprocessorTest : public ::testing::Test
{
protected:
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

    PosChar peek(std::string s, int i)
    {
        std::stringstream ss(s);
        return Preprocessor(ss).peek(i);
    }
};