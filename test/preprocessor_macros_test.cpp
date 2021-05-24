#include "src/preprocessor/preprocessor.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Macros, CanBeDefinedEmpty)
{
    // A trailing comma in args is valid
    std::stringstream input("#define MACRO\n#define MACRO(A,B,C,D,)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n") << "Macro definitions should not appear in output structure";
}

TEST(Macros, CanBeExpandedEmpty)
{
    std::stringstream input("#define MACRO1\nMACRO1");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n") << "Macro expansion should work with an empty body defined";
}

TEST(Macros, CanBeExpandedObjectLike)
{
    std::stringstream input("#define MACRO 1\nMACRO");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n1") << "Macro expansion should work with static content defined";
}

TEST(Macros, CanBeExpandedFunctionLike)
{
    std::stringstream input("#define SEL(A,B,C,) A B C\nSEL([1],select,0)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n[1] select 0") << "Macro expansion should perform parameter replacement";
}

TEST(Macros, StripHorizontalWhitespaceInParams)
{
    std::stringstream input("#define MACRO(  A      ,		B,  ) A + B\nMACRO(1,1)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n1 + 1") << "Macro definition with parameters should strip their surrounding horizontal whitespace";
}

TEST(Macros, CanBeExpandedWithArgStringizing)
{
    std::stringstream input("#define _S(A) #A\n_S(2)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\"2\"") << "Macro expansion should stringize parameters with replacement";
}

TEST(Macros, CanBeExpandedWithStaticStringizing)
{
    std::stringstream input("#define _S comment #Y\n_S");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\ncomment \"Y\"") << "Macro expansion should stringize static content";
}

TEST(Macros, CanBeExpandedWithArgConcatenation)
{
    std::stringstream input("#define _C(A,B) A##B\n_C(20,1)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n201") << "Macro expansion should concatenate parameters with replacement";
}

TEST(Macros, CanBeExpandedWithStaticConcatenation)
{
    std::stringstream input("#define _C A##B\n_C");
    Preprocessor pp(input);

    // TODO fix case of source not persisting through to output
    EXPECT_EQ(pp.processAll(), "\nAB") << "Macro expansion should concatenate static content";
}

TEST(Macros, CanBeExpandedWithConcatenationBesideStringizing)
{
    std::stringstream input("#define test(var1,var2) #var1###var2\ntest(concat,strings)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\"concat\"\"strings\"") << "Macro expansion should not be confused by concatenation beside stringizing";
}

TEST(Macros, CanBeExpandedWithParenthesesArgs)
{
    std::stringstream input("#define _T(A) #A\n_T((()()()))");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\"(()()())\"") << "Macro expansion should support balanced parantheses within the arguments";
}

TEST(Macros, DISABLED_CanBeNested)
{
    std::stringstream input("#define ONE 1\n#define SUM 3+ONE\nSUM");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\n3 + 1") << "Macro expansion should occur within macro body";
}

TEST(Macros, DISABLED_CanBeNestedStringized)
{
    std::stringstream input("#define TWO 2\n#define ONE #TWO\nONE");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\n\"2\"") << "Nested macros should expand before being stringized";
}

TEST(Macros, DISABLED_NestedExpansionBeforeConcatenation)
{
    std::stringstream input("#define TWO 2\n#define ONE T##WO\nONE");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\nTWO") << "Nested macro expansion should occur before concatenation";
}

TEST(Macros, CannotBeRecursive)
{
    std::stringstream input("#define INVALID_RECUR 2 + INVALID_RECUR\nINVALID_RECUR");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n2 + INVALID_RECUR") << "Macro expansion should not recursively expand";
}

TEST(Macros, DISABLED_CanBeNestedOverloaded)
{
    std::stringstream input("#define ONE 1\n#define ONE(A) A+ONE\nONE(3)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\n3 + 1") << "Overloaded macro expansion should occur within macro body";
}

TEST(Macros, AreExpandedAsArgumentsFirst)
{
    std::stringstream input("#define ONE 1\n#define _S(A) #A\n_S(ONE)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n\n\"1\"") << "Macro expansion in arguments should take place before parameter replacement";
}

TEST(Macros, CanBeExpandedWithRecursiveArgs)
{
    std::stringstream input("#define TWO(A, B) A##B\nTWO(TWO(TWO(A,B),C),D)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\nABCD") << "Macro expansion in arguments should support nested argument structures";
}

TEST(Macros, CanContainRawCommentsInArguments)
{
    std::stringstream input("#define TEST(A, B) A B\nTEST(//,/**/)");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\n// /**/") << "Comments within macro arguments should not be removed during expansion";
}