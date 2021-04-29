# cpp-sqf

A C++ implementation of a lexer, parser and interpreter for SQF.

The lexer produces tokens from an input file. The parser consumes the tokens and constructs an abstract syntax tree (AST). The interpreter traverses the AST by means of the visitor pattern and mutates its own state accordingly.

**Author Note**

This is the first C++ I have ever written. My intentions for this project are:

- To familarising myself with C++.
- To learning about how parsers and interpreters work.
- To simultaneously produce something that could be useful.

## Lexical Analysis

- In SQF a program consists of a sequence of statements. The end of a statement (except from the very last) is marked by a `,` or `;` character except where such characters are allowed by syntax (e.g. commas within an array literal).
- A comment starts with a `//` or `/*` character pair that is not part of a string literal, and ends at the end of the physical line or with a `*/` character pair, respectively.
- Outside of string literals, whitespace characters are used to seperate tokens (only necessary if their concatenation could otherwise be interpreted as a different token).

### Identifiers and Keywords

- Identifiers are composed of upper and lowercase letters `A` through `Z`, the underscore `_` and, except for the first character, the digits `0` through `9`.
  - Identifiers are not case sensitive.
  - Identifiers that begin with an underscore are so-called "local variables" and limited to the scope of the script instance (thus can only be used after being initalised in script).
  - Identifiers that begin with a letter are so-called "global variables" and are shared across all running scripts (thus can be used without being initalised in script).
- Many identifiers are reserved as keywords of SQF. Being an unusual language, SQF has many keywords, for an up-to-date list see [the wiki](https://community.bistudio.com/wiki/Category:Scripting_Commands).

### Literals and Displays

- Numeric literals
  - Decimal literals consist of the digits `0` through `9`. Optionally, a single decimal point `.` can also appear anywhere in the literal (start and end included).
  - Hexadecimal literals being with either a `$` or `0x` prefix, followed by digits `0` through `9` and letters `A` through `F` (case insensitive).
  - Scientific notation can extend a decimal literal with the letter `e` (case insensitive), an optional `+` or `-` sign and then further digit characters (must follow).
- String literals are enclosed in matching single or double quotes (`'` or `"`). To use the enclosing character within the string it must be doubled (unlike many languages, `\` cannot be used to escape characters within a string).
- Array displays are enclosed by `[` and `]` characters with expressions separated by `,` characters. There can be no trailing `,` after the last expression.
- Code displays are enclosed by `{` and `}` characters and contain a sequence of statements.

### Operators and Delimiters

- The following tokens are operators: `+  -  /  *  ^  %  >>  ||  &&  ==  !=  >  <  >=  <=  !  #`
- The following tokens serve as delimiters in the grammar: `(  )  [  ]  {  }  ;  ,  =  :`

## Implemented Grammar Rules

The parser produces an intermediate representation (the AST) according to the following SQF grammar. Each rule has a corresponding member function in the parser. Elements in capitals are terminal tokens and should be self explanatory.

**Note:** The grammar was produced by myself and does not necessarily correspond to how the Real Virtuality engine truly parses SQF code.

| Head  | Body |
| --- | --- |
|program|`statement_list EOF`|
|statement_list|`statement ((SEMI\|COMMA) statement)*`|
|statement|`expr \| assignment_statement \| empty`|
|expr|`and (DISJUNCTION and)*`|
|and|`comp (CONJUNCTION comp)*`|
|comp|`binary ((EQL\|NEQL\|GT\|LT\|GTEQL\|LTEQL\|GTGT) binary)*`|
|binary|`conditional (KEYWORD conditional)*`|
|conditional|`term (ELSE term)*`|
|term|`factor ((PLUS\|MINUS\|MIN\|MAX) factor)*`|
|factor|`power ((MUL\|DIV\|MOD\|ATAN2) power)*`|
|power|`hash_select (POW hash_select)*`|
|hash_select|`unary (HASH unary)*`|
|unary|`PLUS unary \| MINUS unary \| NEGATION unary \| KEYWORD unary \| nullary`|
|nullary|`KEYWORD \| variable \| literal \| LPAREN expr RPAREN`|
|literal|`STR_LITERAL \| HEX_LITERAL \| DEC_LITERAL \| array \| code`|
|array|`LSQB expr (, expr)* RSQB`|
|code|`LCURL statement_list RCURL`|
|assignment_statement|`variable ASSIGN expr`|
|variable|`ID`|
|empty||

The following table summarizes the operator precedence in SQF, from highest precedence (most binding) to lowest precedence (least binding). Operators in the same row are equivalent precedence and left associative.

| Operator  | Description |
| --- | --- |
|`commandName`, `(expressions...)`, `[expressions...]`, `{expressions...}`|Nullary operator, parenthesized expression, array display, code display|
|`commandName expression`, `+array`, `!boolean`, `+number`, `-number`|Unary operator|
|`array # number`|Hash select operator|
|`number ^ number`|Exponentiation|
|`*`, `/`, `%`, `mod`, `atan2`, `config / string`|Multiplication, division, modulo, atan2, slash config accessor|
|`+`, `-`, `string + string`, `number min number`, `number max number`|Addition, subtraction, concatenation, min, max|
|`if boolean then code else code`|If – else structure|
|`expression commandName expression`|Binary operator|
|`==`, `!=`, `>`, `<`, `<=`, `>=`, `config >> string`|Comparison, arrow config accessor|
|`bool || bool`, `bool or bool`|Logical OR|
|`bool && bool`, `bool and bool`|Logical AND|

## Notable Mentions

Initial implementation achieved thanks to [Ruslan Spivak's blog](https://ruslanspivak.com/lsbasi-part1/) which I highly recommend if you want to learn about writing an interpreter from scratch.

See [Lord Golias' "SQF linter" project](https://github.com/LordGolias/sqf) for a robust Python implementation of an SQF lexer, parser, interpreter and analyzer. It was the original inspiration for me to learn about these topics and implement my own.