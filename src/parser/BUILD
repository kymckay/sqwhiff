cc_library(
    name = "sqf-parser",
    srcs = ["parser.cpp"],
    hdrs = ["syntax_error.h", "parser.h"],
    deps = ["//src/lexer:sqf-lexer", "//src/ast:abstract-syntax-tree"],
    visibility = ["//src/analyzer:__pkg__", "//test:__pkg__"],
)