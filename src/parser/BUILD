cc_library(
    name = "sqf-parser",
    srcs = ["parser.cpp"],
    hdrs = ["parser.h"],
    deps = ["//src/errors:sqf-errors", "//src/lexer:sqf-lexer", "//src/ast:abstract-syntax-tree"],
    visibility = ["//src/analyzer:__pkg__", "//test/parser:__pkg__"],
)
