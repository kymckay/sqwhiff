cc_library(
    name = "sqf-tokens",
    hdrs = ["token_types.h", "token_maps.h", "token.h"],
    visibility = ["//src/ast:__pkg__", "//src/errors:__pkg__"],
)

cc_library(
    name = "sqf-lexer",
    srcs = ["lexer.cpp"],
    hdrs = ["lexer.h"],
    deps = [":sqf-tokens", "//src/sqf:sqf-lang", "//src/errors:sqf-errors", "//src/preprocessor:sqf-preprocessor"],
    visibility = ["//src/parser:__pkg__", "//test/lexer:__pkg__"],
)
