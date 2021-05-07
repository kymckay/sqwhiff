cc_library(
    name = "sqf-tokens",
    hdrs = ["token_types.h", "token_maps.h", "token.h"],
    visibility = ["//src/ast:__pkg__", "//src/errors:__pkg__"],
)

cc_library(
    name = "sqf-lexer",
    srcs = ["lexer.cpp"],
    hdrs = ["lexer.h"],
    deps = [":sqf-tokens", "//src/sqf:sqf-keywords", "//src/errors:sqf-errors"],
    visibility = ["//src/parser:__pkg__"],
)