#include "src/analyzer/analyzer.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

class AnalyzerTest : public ::testing::Test
{
protected:
    std::string output_;
    int errorc_ = -1;

    void analyze(std::string s)
    {
        std::stringstream input(s);
        std::stringstream output("");
        Preprocessor pp(input);
        Lexer l(pp);
        Parser p(l);
        Analyzer a(p, output);
        errorc_ = a.analyze();

        output_ = output.str();
    }

    void SetUp() override {
        // Reset this to an invalid value avoids leakage between tests
        errorc_ = -1;
    }
};