load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "sqf-ast-tester",
    srcs = ["tester.cpp"],
    hdrs = ["tester.h"],
    deps = ["//src/parser:sqf-parser"]
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
  deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "semantic-errors",
  size = "small",
  srcs = ["semantic_errors_test.cpp"],
  deps = ["//src/analyzer:sqf-analyzer", "@com_google_googletest//:gtest_main"],
)