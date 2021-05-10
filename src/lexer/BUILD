cc_library(
    name = "sqf-tokens",
    hdrs = ["token_types.h", "token_maps.h", "token.h"],
    visibility = ["//src/ast:__pkg__", "//src/errors:__pkg__"],
)

cc_library(
    name = "sqf-lexer",
    srcs = ["lexer.cpp"],
    hdrs = ["lexical_error.h", "lexer.h"],
    deps = [":sqf-tokens", "//src/sqf:sqf-keywords"],
    visibility = ["//src/parser:__pkg__"],
)