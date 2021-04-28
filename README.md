# cpp-sqf

A C++ implementation of a lexer, parser and interpreter for SQF.

The lexer produces tokens from an input file. The parser consumes the tokens and constructs an abstract syntax tree (AST). The interpreter traverses the AST by means of the visitor pattern and mutates its own state accordingly.

**Author Note**

This is the first C++ I have ever written. My intentions for this project are:

- To familarising myself with C++.
- To learning about how parsers and interpreters work.
- To simultaneously produce something that could be useful.

## Lexical Analysis

- In SQF a program consists of a sequence of expressions. The end of an expression is marked by a `,` or `;` character except where such characters are allowed by syntax (e.g. commas within an array literal).
- A comment starts with a `//` or `/*` character pair that is not part of a string literal, and ends at the end of the physical line or with a `*/` character pair, respectively.
- Outside of string literals, whitespace characters are used to seperate tokens (only necessary if their concatenation could otherwise be interpreted as a different token).

### Identifiers and Keywords

- Identifiers are composed of upper and lowercase letters `A` through `Z`, the underscore `_` and, except for the first character, the digits `0` through `9`.
  - Identifiers are not case sensitive.
  - Identifiers that begin with an underscore are so-called "local variables" and limited to the scope of the script instance (thus can only be used after being initalised in script).
  - Identifiers that begin with a letter are so-called "global variables" and are shared across all running scripts (thus can be used without being initalised in script).
- Many identifiers are reserved as keywords of SQF. Being an unusual language, SQF has many keywords, for an up-to-date list see [the wiki](https://community.bistudio.com/wiki/Category:Scripting_Commands).

### Literals

- Numeric literals
  - Decimal literals consist of the digits `0` through `9`. Optionally, a single decimal point `.` can also appear anywhere in the literal (start and end included).
  - Hexadecimal literals being with either a `$` or `0x` prefix, followed by digits `0` through `9` and letters `A` through `F` (case insensitive).
  - Scientific notation can extend either of the above literal with the letter `e`, an optional `+` or `-` sign and further allowable characters for the respective type.
- String literals are enclosed in matching single or double quotes (`'` or `"`). To use the enclosing character within the string it must be doubled (unlike many languages, `\` cannot be used to escape characters within a string).
- Array literals are enclosed by `[` and `]` characters with elements separated by `,` characters.
- Code literals are enclosed by `{` and `}` characters and contain a sequence of expressions.

### Operators and Delimiters

- The following tokens are operators: `+  -  /  *`
- The following tokens serve as delimiters in the grammar: `(  )  ;  ,  =`

## Implemented Grammar Rules

The parser produces an intermediate representation (the AST) according to the following SQF grammar. Each rule has a corresponding member function in the parser. Elements in capitals are terminal tokens and should be self explanatory.

**Note:** The grammar was produced by myself and does not necessarily correspond to how the Real Virtuality engine truly parses SQF code.

| Head  | Body |
| --- | --- |
|program|`statement_list EOF`|
|statement_list|`statement ((SEMI\|COMMA) statement)*`|
|statement|`assignment_statement \| empty`|
|assignment_statement|`variable ASSIGN expr`|
|variable|`ID`|
|expr|`term ((PLUS\|MINUS) term)*`|
|term|`factor ((MUL\|DIV) factor)*`|
|factor|`PLUS factor \| MINUS factor \| NUMBER \| LPAREN expr RPAREN \| variable`|
|empty||

## Notable Mentions

Initial implementation achieved thanks to [Ruslan Spivak's blog](https://ruslanspivak.com/lsbasi-part1/) which I highly recommend if you want to learn about writing an interpreter from scratch.

See [Lord Golias' "SQF linter" project](https://github.com/LordGolias/sqf) for a robust Python implementation of an SQF lexer, parser, interpreter and analyzer. It was the original inspiration for me to learn about these topics and implement my own.