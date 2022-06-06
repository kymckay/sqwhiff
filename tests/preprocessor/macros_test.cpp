#include "tests/preprocessor/_test.hpp"

TEST_F(PreprocessorTest, CanBeDefinedEmpty) {
  // A trailing comma in params is valid
  EXPECT_EQ("\n", preprocess("#define MACRO\n#define MACRO(A,B,C,D,)"))
      << "Macro definitions should not appear in output structure";
}

TEST_F(PreprocessorTest, ExpandsMacrosEmpty) {
  EXPECT_EQ("\n", preprocess("#define MACRO1\nMACRO1"))
      << "Macro expansion should work with an empty body defined";
}

TEST_F(PreprocessorTest, ExpandsMacrosObjectLike) {
  EXPECT_EQ("\n1", preprocess("#define MACRO 1\nMACRO"))
      << "Macro expansion should work with static content defined";
}

TEST_F(PreprocessorTest, ExpandsMacrosFunctionLike) {
  EXPECT_EQ("\n[1] select 0",
            preprocess("#define SEL(A,B,C,) A B C\nSEL([1],select,0)"))
      << "Macro expansion should perform parameter replacement";
}

TEST_F(PreprocessorTest, StripHorizontalWhitespaceInParams) {
  EXPECT_EQ(
      "\n1 + 1",
      preprocess(
          "#define MACRO(  A      ,		B,  ) A + B\nMACRO(1,1)"))
      << "Macro definition with parameters should strip their surrounding "
         "horizontal whitespace";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithArgStringizing) {
  EXPECT_EQ("\n\"2\"", preprocess("#define _S(A) #A\n_S(2)"))
      << "Macro expansion should stringize parameters with replacement";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithStaticStringizing) {
  EXPECT_EQ("\ncomment \"Y\"", preprocess("#define _S comment #Y\n_S"))
      << "Macro expansion should stringize static content";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithArgConcatenation) {
  EXPECT_EQ("\n201", preprocess("#define _C(A,B) A##B\n_C(20,1)"))
      << "Macro expansion should concatenate parameters with replacement";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithStaticConcatenation) {
  EXPECT_EQ("\nAB", preprocess("#define _C A##B\n_C"))
      << "Macro expansion should concatenate static content";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithConcatenationBesideStringizing) {
  EXPECT_EQ("\n\"concat\"\"strings\"",
            preprocess(
                "#define _cats(var1,var2) #var1###var2\n_cats(concat,strings)"))
      << "Macro expansion should not be confused by concatenation beside "
         "stringizing";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithParenthesesArgs) {
  EXPECT_EQ("\n\"(()()())\"", preprocess("#define _T(A) #A\n_T((()()()))"))
      << "Macro expansion should support balanced parantheses within the "
         "arguments";
}

TEST_F(PreprocessorTest, ExpandsNestedMacros) {
  EXPECT_EQ("\n\n3+1", preprocess("#define ONE 1\n#define SUM 3+ONE\nSUM"))
      << "Macro expansion should occur within macro body";
}

TEST_F(PreprocessorTest, ExpandsNestedMacrosStringized) {
  EXPECT_EQ("\n\n\"2\"", preprocess("#define TWO 2\n#define ONE #TWO\nONE"))
      << "Nested macros should expand before being stringized";
}

TEST_F(PreprocessorTest, ExpandsNestedMacrosOutOfOrder) {
  EXPECT_EQ("\n\n2", preprocess("#define ONE TWO\n#define TWO 2\nONE"))
      << "A nested macro should be expanded as defined at the point when it is "
         "used";
}

TEST_F(PreprocessorTest, ExpandsNestedMacrosBeforeConcatenation) {
  EXPECT_EQ("\n\nTWO", preprocess("#define TWO 2\n#define ONE T##WO\nONE"))
      << "Nested macro expansion should occur before concatenation";
}

TEST_F(PreprocessorTest, DoesNotExpandRecusriveMacroUse) {
  EXPECT_EQ("\n\n\n\n1 + 2 + 3 + 4 + ONE + TWO + THR + FOU",
            preprocess("#define FOU 4 + ONE + TWO + THR + FOU\n#define THR 3 + "
                       "FOU\n#define TWO 2 + THR\n#define ONE 1 + TWO\nONE"))
      << "Macro expansion should not recursively expand any earlier link in "
         "the chain";
}

TEST_F(PreprocessorTest, ExpandsArgumentsBeforeReplacement) {
  EXPECT_EQ("\n\n\"1\"", preprocess("#define ONE 1\n#define _S(A) #A\n_S(ONE)"))
      << "Macro expansion in arguments should take place before parameter "
         "replacement";
}

TEST_F(PreprocessorTest, ExpandsMacrosWithRecursiveArgs) {
  EXPECT_EQ("\nABCD",
            preprocess("#define TWO(A, B) A##B\nTWO(TWO(TWO(A,B),C),D)"))
      << "Macro expansion in arguments should support nested argument "
         "structures";
}

TEST_F(PreprocessorTest, CanContainRawCommentsInArguments) {
  EXPECT_EQ("\n// /**/", preprocess("#define RAW(A, B) A B\nRAW(//,/**/)"))
      << "Comments within macro arguments should not be removed during "
         "expansion";
}

TEST_F(PreprocessorTest, CanContainRawDirectivesInArguments) {
  EXPECT_EQ("\n \n#define NEW(A) A",
            preprocess("#define RAW(A, B) A B\nRAW(,\n#define NEW(A) A)"))
      << "Directives within macro arguments should not be preprocessed during "
         "expansion";
}

TEST_F(PreprocessorTest, CanUndefineMacros) {
  EXPECT_EQ("\n\nTEST", preprocess("#define TEST 35\n#undef TEST\nTEST"))
      << "Undefined macros should not be expanded";
}

TEST_F(PreprocessorTest, UndefinesMacrosWhereEncountered) {
  EXPECT_EQ("\n35\n\nTEST",
            preprocess("#define TEST 35\nTEST\n#undef TEST\nTEST"))
      << "Undefined macros should not be expanded only after line where "
         "undefined";
}

TEST_F(PreprocessorTest, CanUndefineUndefinedMacro) {
  EXPECT_EQ("\n", preprocess("#undef UNDEFINED\n"))
      << "Undefine directives for already undefined macros should silently do "
         "nothing";
}

TEST_F(PreprocessorTest, DISABLED_OnlyUndefinesFirstWord) {
  EXPECT_EQ("\n 1", preprocess("#define TRAILING 1\n#undef MACRO TRAILING"))
      << "An undefine directive should only consume the first following word";
}

TEST_F(PreprocessorTest, CanRedefineMacro) {
  EXPECT_EQ("\n1\n\n2",
            preprocess("#define TEST 1\nTEST\n#define TEST 2\nTEST"))
      << "The last define encountered overrides all earlier macro definitions";
}
