cc_library(
    name = "sqf-tokens",
    hdrs = ["token_type.h", "token.h"],
    visibility = ["//src/ast:__pkg__"],
)

cc_library(
    name = "sqf-lexer",
    srcs = ["lexer.cpp"],
    hdrs = ["lexer.h"],
    deps = [":sqf-tokens", "//src/sqf:sqf-keywords"],
    visibility = ["//src/parser:__pkg__"],
)