load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "sqf-ast-tester",
    srcs = ["tester.cpp"],
    hdrs = ["tester.h"],
    deps = ["//src/parser:sqf-parser"]
)

cc_library(
    name = "sqf-preproc-test",
    hdrs = ["test_preprocessor.h"],
    deps = ["//src/preprocessor:sqf-preprocessor"]
)

cc_test(
  name = "analyzer-arity",
  size = "small",
  srcs = ["analyzer_arity_test.cpp"],
  deps = ["//src/analyzer:sqf-analyzer", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "lexer-errors",
  size = "small",
  srcs = ["lexer_errors_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "lexer-keywords",
  size = "small",
  srcs = ["lexer_keywords_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-errors",
  size = "small",
  srcs = ["parser_errors_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-literals",
  size = "small",
  srcs = ["parser_literals_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-precedence",
  size = "small",
  srcs = ["parser_precedence_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-private",
  size = "small",
  srcs = ["parser_private_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-program",
  size = "small",
  srcs = ["parser_program_test.cpp"],
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "preprocessor-comments",
  size = "small",
  srcs = ["preprocessor_comments_test.cpp"],
  deps = [":sqf-preproc-test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "preprocessor-directive",
  size = "small",
  srcs = ["preprocessor_directive_test.cpp"],
  deps = [":sqf-preproc-test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "preprocessor-macros",
  size = "small",
  srcs = ["preprocessor_macros_test.cpp"],
  deps = [":sqf-preproc-test", "@com_google_googletest//:gtest_main"],
)