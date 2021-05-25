load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_test.h"],
    deps = ["//src/analyzer:sqf-analyzer", "@com_google_googletest//:gtest_main"]
)

cc_test(
    name = "arity",
    size = "small",
    srcs = ["arity_test.cpp"],
    deps = [":test"],
)
