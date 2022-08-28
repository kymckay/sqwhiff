load("@rules_cc//cc:defs.bzl", "cc_test")

# Libraries with fixtures and helper methods to facilitate tests
cc_library(
    name = "pp-test",
    hdrs = ["preprocessor/_test.hpp"],
    deps = ["//sqwhiff:preprocessor", "@com_google_googletest//:gtest_main"]
)

cc_library(
  name = "lexer-test",
  hdrs = ["lexer/_test.hpp"],
  deps = ["//sqwhiff:lexer", "@com_google_googletest//:gtest_main"]
)

cc_library(
  name = "parser-test",
  hdrs = ["parser/_tester.hpp", "parser/_test.hpp"],
  deps = ["//sqwhiff:parser", "@com_google_googletest//:gtest_main"]
)

cc_library(
  name = "analyzer-test",
  hdrs = ["analyzer/_test.hpp"],
  deps = ["//sqwhiff:analyzer", "@com_google_googletest//:gtest_main"],
)

# Preprocessor tests
cc_test(
  name = "pp-branches",
  size = "small",
  srcs = ["preprocessor/branches_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-comments",
  size = "small",
  srcs = ["preprocessor/comments_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-directives",
  size = "small",
  srcs = ["preprocessor/directives_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-errors",
  size = "small",
  srcs = ["preprocessor/errors_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-error-positions",
  size = "small",
  srcs = ["preprocessor/error_positions_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-include",
  size = "small",
  srcs = ["preprocessor/include_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-macros",
  size = "small",
  srcs = ["preprocessor/macros_test.cpp"],
  deps = [":pp-test"],
)

cc_test(
  name = "pp-peek",
  size = "small",
  srcs = ["preprocessor/peek_test.cpp"],
  deps = [":pp-test"],
)

# Lexer tests
cc_test(
  name = "lexer-errors",
  size = "small",
  srcs = ["lexer/errors_test.cpp"],
  deps = [":lexer-test"],
)

cc_test(
  name = "lexer-keywords",
  size = "small",
  srcs = ["lexer/keywords_test.cpp"],
  deps = [":lexer-test"],
)

cc_test(
  name = "lexer-peek",
  size = "small",
  srcs = ["lexer/peek_test.cpp"],
  deps = [":lexer-test"],
)

# Parser tests
cc_test(
  name = "parser-control-flow",
  size = "small",
  srcs = ["parser/control_flow_test.cpp"],
  deps = [":parser-test"],
)

cc_test(
  name = "parser-errors",
  size = "small",
  srcs = ["parser/errors_test.cpp"],
  deps = [":parser-test"],
)

cc_test(
  name = "parser-literals",
  size = "small",
  srcs = ["parser/literals_test.cpp"],
  deps = [":parser-test"],
)

cc_test(
  name = "parser-precedence",
  size = "small",
  srcs = ["parser/precedence_test.cpp"],
  deps = [":parser-test"],
)

cc_test(
  name = "parser-private",
  size = "small",
  srcs = ["parser/private_test.cpp"],
  deps = [":parser-test"],
)

cc_test(
  name = "parser-program",
  size = "small",
  srcs = ["parser/program_test.cpp"],
  deps = [":parser-test"],
)

# Analyzer tests
cc_test(
  name = "analyzer",
  size = "small",
  srcs = ["analyzer/analyze_test.cpp"],
  deps = [":analyzer-test"],
)

# Rule tests
cc_test(
    name = "rules-arity",
    size = "small",
    srcs = ["rules/arity_test.cpp"],
    deps = [":analyzer-test", "//sqwhiff:rules"],
)

cc_test(
    name = "rules-type",
    size = "small",
    srcs = ["rules/type_test.cpp"],
    deps = [":analyzer-test", "//sqwhiff:rules"],
)
