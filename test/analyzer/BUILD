load("@rules_cc//cc:defs.bzl", "cc_test")

cc_library(
    name = "test",
    hdrs = ["_test.h"],
    deps = ["//src/analyzer:sqf-analyzer", "@com_google_googletest//:gtest_main"],
    visibility = ["//test/rules:__pkg__"]
)

cc_test(
    name = "analyze",
    size = "small",
    srcs = ["analyze_test.cpp"],
    deps = [":test"],
)
