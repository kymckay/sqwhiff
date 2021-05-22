#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Macros, CanBeDefinedEmpty)
{
    std::stringstream input("#define MACRO\n");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro definition should not appear in output structure";
}

TEST(Macros, CanBeDefinedWithArgs)
{
    // A trailing comma is valid
    std::stringstream input("#define MACRO(A,B,C,D,)\n");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro definition with parameters should not appear in output structure";
}

TEST(Macros, CanBeExpandedEmpty)
{
    std::stringstream input("#define MACRO1\nMACRO1");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro expansion should work with no arguments";
}

TEST(Macros, CanBeExpandedToStatic)
{
    std::stringstream input("#define MACRO 1\nMACRO");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Dec:1>") << "Macro expansion should work with static content";
}

TEST(Macros, CanBeExpandedWithArgs)
{
    // A trailing comma is valid
    std::stringstream input("#define MACRO(A,B,C,D,)\nMACRO(1,2,3,4)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro expansion should work with arguments";
}

TEST(Macros, CanBeExpandedWithArgReplacement)
{
    std::stringstream input("#define SEL(A,B,C) A B C\nSEL([1],select,0)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "([<Dec:1>] select <Dec:0>)") << "Macro expansion should perform parameter replacement";
}

TEST(Macros, CanBeExpandedWithArgStringification)
{
    std::stringstream input("#define _S(A) #A\n_S(2)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Str:2>") << "Macro expansion should stringify parameters with replacement";
}

TEST(Macros, CanBeExpandedWithStaticStringification)
{
    std::stringstream input("#define _S comment #Y\n_S");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(comment <Str:Y>)") << "Macro expansion should stringify static content";
}

TEST(Macros, CanBeExpandedWithArgConcatenation)
{
    std::stringstream input("#define _C(A,B) A##B\n_C(20,1)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Dec:201>") << "Macro expansion should concatenate parameters with replacement";
}

TEST(Macros, CanBeExpandedWithStaticConcatenation)
{
    std::stringstream input("#define _C A##B\n_C");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    // TODO fix case of source not persisting through to output
    EXPECT_EQ(t.test(), "<Var:ab>") << "Macro expansion should concatenate static content";
}

TEST(Macros, CanBeExpandedWithConcatenationBesideStringification)
{
    std::stringstream input("#define test(var1,var2) #var1###var2\ntest(concat,strings)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Str:concat\"strings>") << "Macro expansion should not be confused by concatenation beside stringification";
}

TEST(Macros, DISABLED_CanBeNested)
{
    std::stringstream input("#define ONE 1\n#define SUM 3+ONE\nSUM");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(<Dec:3> + <Dec:1)") << "Macro expansion should occur within macro body";
}

TEST(Macros, CannotBeRecursive)
{
    std::stringstream input("#define INVALID_RECUR 2 + INVALID_RECUR\nINVALID_RECUR");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(<Dec:2> + <Var:invalid_recur>)") << "Macro expansion should not recursively expand";
}

TEST(Macros, DISABLED_CanBeNestedOverloaded)
{
    std::stringstream input("#define ONE 1\n#define ONE(A) A+ONE\nONE(3)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "(<Dec:3> + <Dec:1)") << "Overloaded macro expansion should occur within macro body";
}

TEST(Macros, DISABLED_AreExpandedAsArgumentsFirst)
{
    std::stringstream input("#define ONE 1\n#define _S(A) #A\n_S(ONE)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Str:1>") << "Macro expansion in arguments should take place before parameter replacement";
}