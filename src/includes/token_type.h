#pragma once

// Tokens the parser must know about to create the AST structure
enum class TokenType
{
    // misc
    unknown,
    id,      // variable name
    keyword, // generic sqf command
    end_of_file,
    // operators
    plus,
    minus,
    mul,
    div,
    pow,
    mod,
    hash,
    not,
    or,
    and,
    eql,
    neql,
    gt,
    lt,
    gteql,
    lteql,
    gtgt,
    // delimiters
    lparen,
    rparen,
    assign,
    semi,
    comma,
    // literals
    dec_literal,
    hex_literal,
    // reserved keywords with different precedence
    atan2
};