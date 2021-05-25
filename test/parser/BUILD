load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_tester.h", "_test.h"],
    deps = ["//src/parser:sqf-parser"]
)

cc_test(
  name = "parser-errors",
  size = "small",
  srcs = ["errors_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-literals",
  size = "small",
  srcs = ["literals_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-precedence",
  size = "small",
  srcs = ["precedence_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-private",
  size = "small",
  srcs = ["private_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)

cc_test(
  name = "parser-program",
  size = "small",
  srcs = ["program_test.cpp"],
  deps = [":test", "@com_google_googletest//:gtest_main"],
)
