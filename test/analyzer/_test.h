#include "src/analyzer/analyzer.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

class AnalyzerTest : public ::testing::Test
{
protected:
    std::string output_;

    void analyze(std::string s)
    {
        std::stringstream input(s);
        std::stringstream output("");
        Preprocessor pp(input);
        Lexer l(pp);
        Parser p(l);
        Analyzer a(p, output);
        a.analyze();

        output_ = output.str();
    }
};