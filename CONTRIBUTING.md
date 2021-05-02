# How to contribute

As this is a learning project for myself, please make any feature/enhancement requests as issues for review as I'd like the chance to implement them myself or mark issues as "help wanted" where appropriate.

If you'd like to fix a bug or optimise my implementation then by all means submit a pull request for review and explain why it's better/how it works.

## Build setup

The project is configured to be built using [Bazel](https://www.bazel.build). For simplicity, I recommend you use [Bazelisk](https://github.com/bazelbuild/bazelisk) to ensure you're running the correct version at all times (the project contains a version file to control this).

You can then build with the following command once `bazel.exe` is on your system path: `bazel build //src/main:sqf`