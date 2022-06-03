cc_library(
    name = "argparser",
    hdrs = ["argparser.h"],
    srcs = ["argparser.cpp"]
)

cc_binary(
    name = "sqwhiff",
    srcs = ["sqwhiff.cpp"],
    deps = ["//src/rules:sqf-rules", ":argparser"],
)
