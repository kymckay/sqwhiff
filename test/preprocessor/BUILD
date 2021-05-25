load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_test.h"],
    deps = ["//src/preprocessor:sqf-preprocessor", "@com_google_googletest//:gtest_main"]
)

cc_test(
  name = "comments",
  size = "small",
  srcs = ["comments_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "directives",
  size = "small",
  srcs = ["directives_test.cpp"],
  deps = [":test"],
)

cc_test(
  name = "macros",
  size = "small",
  srcs = ["macros_test.cpp"],
  deps = [":test"],
)