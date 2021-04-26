# sqf-cpp

A C++ implementation of a lexer, parser and interpreter for SQF.

The lexer produces tokens from an input file. The parser consumes the tokens and constructs an abstract syntax tree (AST). The interpreter traverses the AST by means of the visitor pattern and mutates its state.

### Author Note

This is the first C++ I've ever written. My intentions for this project are:

- To familarising myself with C++.
- To learning about how parsers and interpreters work.
- To simultaneously produce something that could be useful.

## Implemented Grammar Rules

The parser produces an intermediate representation (the AST) according to the following grammar. Each rule has a corresponding member function in the parser. Elements in capitals are terminal tokens and should be self explanatory.

| Head  | Body |
| --- | --- |
|program|`statement_list EOF`|
|statement_list|`statement \| statement SEMI statement_list \| statement COMMA statement_list`|
|statement|`assignment_statement \| empty`|
|assignment_statement|`variable ASSIGN expr`|
|variable|`ID`|
|expr|`term ((PLUS\|MINUS) term)*`|
|term|`factor ((MUL\|DIV) factor)*`|
|factor|`PLUS factor \| MINUS factor \| NUMBER \| LPAREN expr RPAREN \| variable`|
|empty|``|

## Notable Mentions

Initial implementation achieved thanks to [Ruslan Spivak's blog](https://ruslanspivak.com/lsbasi-part1/) which I highly recommend if you want to learn about writing an interpreter from scratch.

See [Lord Golias' "SQF linter" project](https://github.com/LordGolias/sqf) for a robust Python implementation of an SQF lexer, parser, interpreter and analyzer. It was the original inspiration for me to learn about these topics and implement my own.