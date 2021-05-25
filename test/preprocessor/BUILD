load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_test.h"],
    deps = ["//src/preprocessor:sqf-preprocessor"]
)

cc_test(
  name = "preprocessor-comments",
  size = "small",
  srcs = ["comments_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "preprocessor-directives",
  size = "small",
  srcs = ["directives_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "preprocessor-macros",
  size = "small",
  srcs = ["macros_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)