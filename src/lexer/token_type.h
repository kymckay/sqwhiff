#pragma once

// Tokens the parser must know about to create the AST structure
enum class TokenType
{
    // misc
    unknown,
    id,      // variable name
    nullary, // nullary keyword (differentiated to avoid grammar ambiguity, see issue #11)
    keyword, // unary or binary sqf keyword
    private_op, // private is special and can be an assignment modifier
    end_of_file,
    // operations
    plus,
    minus,
    mul,
    div,
    pow,
    mod,
    atan2,
    min,
    max,
    hash,
    negation,
    disjunction,
    conjunction,
    eql,
    neql,
    gt,
    lt,
    gteql,
    lteql,
    gtgt,
    else_op,
    // delimiters
    lparen,
    rparen,
    lsqb,
    rsqb,
    lcurl,
    rcurl,
    assign,
    semi,
    comma,
    // literals
    dec_literal,
    hex_literal,
    str_literal
};