load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_test.h"],
    deps = ["//src/lexer:sqf-lexer", "@com_google_googletest//:gtest_main"]
)

cc_test(
  name = "errors",
  size = "small",
  srcs = ["errors_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "keywords",
  size = "small",
  srcs = ["keywords_test.cpp"],
  deps = [":test"],
)