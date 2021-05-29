#pragma once
#include "src/lexer/lexer.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <sstream>

class LexerTest : public ::testing::Test
{
protected:
    std::vector<Token> tokens;

    void tokenize(std::string s)
    {
        std::stringstream ss(s);
        Preprocessor pp(ss);
        Lexer l(pp);

        Token t = l.get();
        while (t.type != TokenType::end_of_file)
        {
            tokens.push_back(t);
            t = l.get();
        }
    }

    Token peek(std::string s, int i)
    {
        std::stringstream ss(s);
        Preprocessor pp(ss);
        return Lexer(pp).peek(i);
    }

    TokenType getType(int i)
    {
        return tokens[i].type;
    }

    std::string getStr(int i)
    {
        return tokens[i].raw;
    }
};