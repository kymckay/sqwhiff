load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "analyzer-arity",
  size = "small",
  srcs = ["analyzer_arity_test.cpp"],
  deps = ["//src/analyzer:sqf-analyzer", "@com_google_googletest//:gtest_main"],
)

# cc_test(
#   name = "lexer-errors",
#   size = "small",
#   srcs = ["lexer_errors_test.cpp"],
#   deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
# )

# cc_test(
#   name = "lexer-keywords",
#   size = "small",
#   srcs = ["lexer_keywords_test.cpp"],
#   deps = [":sqf-ast-tester", "@com_google_googletest//:gtest_main"],
# )