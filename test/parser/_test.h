#pragma once
#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "test/parser/_tester.h"

class ParserTest : public ::testing::Test {
 protected:
  std::string parse(std::string s) {
    std::stringstream ss(s);
    Preprocessor pp(ss);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    return t.test();
  }
};
