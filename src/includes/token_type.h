#pragma once

// Tokens the parser must know about to create the AST structure
enum class TokenType
{
    // misc
    unknown,
    id,      // variable name
    keyword, // generic sqf command
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
    hex_literal
};