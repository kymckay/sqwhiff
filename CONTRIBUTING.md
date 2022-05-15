# How to contribute

It's always preferable to use the issue tracker before working on anything to avoid duplicating work. In general:

1. Check if an existing issue covers your topic. Check the discussion and comment there if it does.
  - If you would like to work on an issue, first check if anyone else is (either assigned or in the discussion) and make your intentions known.
  - Issues marked "help wanted" are any deemed desirable, but not prioritised.
2. If no issues cover your topic, open a new issue and describe your problem or proposal concisely. Then wait for a response.

If you'd like to optimise any existing implementation make sure to explain why it's better/how it works. Ideally with some demonstrable results included.

## Source code

The project is written using modern C++. Officially C++17 standard, but that may be bumped up in future.

The source files are structured into subdirectories (by logical divisions of responsibility) under the `src` directory.

## Build setup

The project is configured to be built using [Bazel]. For simplicity, I recommend you use [Bazelisk] to ensure you're running the correct version at all times (the project contains a version file to control this).

You can then build with the following command once the Bazel binary is on your system path: `bazel build //src/main:sqwhiff`

## Testing

Tests (found under the `test` directory) are written using [GoogleTest]. When a bug is fixed an equivalent test case should always be added to avoid future regression. Similarly, when a new feature is added appropriate tests should be included to capture intended behaviours.

You can run the full test collection with `bazel test //test/...`.

A GitHub Action is configured to run all tests on pushes to `main` and for pull requests targeting `main`. Pull requests must build and pass testing before they can be merged.

[Bazel]: https://www.bazel.build
[Bazelisk]: https://github.com/bazelbuild/bazelisk
[GoogleTest]: https://github.com/google/googletest
