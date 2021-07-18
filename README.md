# sqwhiff

![](https://github.com/kymckay/sqwhiff/actions/workflows/test.yml/badge.svg)

A C++ implementation of a preprocessor, lexer, parser and semantic analyzer for the Real Virtuality engine's SQF scripting language.

The preprocessor produces a stream of characters from an input file. The lexer consumes the characters to produce a stream of tokens. The parser consumes the tokens and constructs an abstract syntax tree (AST). The analyzer traverses the AST by means of the visitor pattern and checks for (both static and dynamic) semantic errors while modifying its state (i.e. performing limited interpretation).

**Author Note**

This is the first C++ I have ever written. My intentions for this project are:

- To familarising myself with C++.
- To learning about how parsers and interpreters work.
- To simultaneously produce something that could be useful.

If you'd like to contribute please read [CONTRIBUTING.md](CONTRIBUTING.md).

For technical details of the language implementation used, see [LANGUAGE.md](LANGUAGE.md).

## Notable Mentions

Initial implementation achieved thanks to [Ruslan Spivak's blog](https://ruslanspivak.com/lsbasi-part1/) which I highly recommend if you want to learn about writing an interpreter from scratch.

See [Lord Golias' "SQF linter" project](https://github.com/LordGolias/sqf) for a robust Python implementation of an SQF lexer, parser, interpreter and analyzer. It was the original inspiration for me to learn about these topics and implement my own.