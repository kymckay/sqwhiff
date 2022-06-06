cc_library(
    name = "errors",
    hdrs = glob(["errors/*.hpp"]),
    srcs = glob(["errors/*.cpp"]),
    deps = [":tokens"],
)

cc_library(
    name = "preprocessor",
    hdrs = glob(["preprocessor/*.hpp"]),
    srcs = glob(["preprocessor/*.cpp"]),
    deps = [":errors"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "sqf-lang",
    hdrs = glob(["sqf/*.hpp"]),
)

cc_library(
    name = "tokens",
    hdrs = glob(["tokens/*.hpp"]),
)

cc_library(
    name = "lexer",
    hdrs = ["lexer/lexer.hpp"],
    srcs = ["lexer/lexer.cpp"],
    deps = [":tokens", ":sqf-lang", ":errors", ":preprocessor"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "abstract-syntax-tree",
    hdrs = glob(["ast/*.hpp"]),
    deps = [":tokens"],
)

cc_library(
    name = "parser",
    hdrs = ["parser/parser.hpp"],
    srcs = ["parser/parser.cpp"],
    deps = [":errors", ":lexer", ":abstract-syntax-tree"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "analyzer",
    hdrs = ["analyzer/analyzer.hpp", "analyzer/rule.hpp"],
    srcs = ["analyzer/analyzer.cpp", "analyzer/rule.cpp"],
    deps = [":parser"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "rules",
    hdrs = glob(["rules/*.hpp"]),
    srcs = glob(["rules/*.cpp"]),
    deps = [":sqf-lang", ":analyzer"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "argparser",
    hdrs = ["argparser/argparser.hpp"],
    srcs = ["argparser/argparser.cpp"],
)

cc_binary(
    name = "sqwhiff",
    srcs = ["main.cpp"],
    deps = [":argparser", ":rules"],
)
