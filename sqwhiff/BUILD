# Components
cc_library(
    name = "structures",
    hdrs = glob(["structures/*.hpp"]),
    srcs = glob(["structures/*.cpp"], ["structures/*.test.cpp"]),
)

cc_library(
    name = "errors",
    hdrs = glob(["errors/*.hpp"]),
    srcs = glob(["errors/*.cpp"]),
    deps = [":structures", ":tokens"],
)

cc_library(
    name = "preprocessor",
    hdrs = glob(["preprocessor/*.hpp"]),
    srcs = glob(["preprocessor/*.cpp"],  ["preprocessor/*.test.cpp"]),
    deps = [":structures", ":errors"],
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
    name = "rules",
    hdrs = glob(["rules/*.hpp"]),
    srcs = glob(["rules/*.cpp"]),
    deps = [":abstract-syntax-tree", ":errors", ":sqf-lang"],
    visibility = ["//tests:__pkg__"],
)

cc_library(
    name = "analyzer",
    hdrs = ["analyzer/analyzer.hpp"],
    srcs = ["analyzer/analyzer.cpp"],
    deps = [":parser", ":rules"],
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
    deps = [":argparser", ":analyzer"],
)

# Unit tests
cc_test(
    name = "test-structures",
    size = "small",
    srcs = glob(["structures/*.test.cpp"]),
    deps = [":structures", "@com_google_googletest//:gtest_main"],
)

cc_test(
    name = "test-preprocessor",
    size = "small",
    srcs = glob(["preprocessor/*.test.cpp"]),
    deps = [":preprocessor", "@com_google_googletest//:gtest_main"],
)
