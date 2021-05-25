load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_tester.h", "_test.h"],
    deps = ["//src/parser:sqf-parser", "@com_google_googletest//:gtest_main"]
)

cc_test(
  name = "errors",
  size = "small",
  srcs = ["errors_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "literals",
  size = "small",
  srcs = ["literals_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "precedence",
  size = "small",
  srcs = ["precedence_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "private",
  size = "small",
  srcs = ["private_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "program",
  size = "small",
  srcs = ["program_test.cpp"],
  deps = [":test"],
)
