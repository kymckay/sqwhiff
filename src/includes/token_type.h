#pragma once

// Tokens the parser must know about to create the AST structure
enum class TokenType
{
    unknown,
    dec_literal,
    hex_literal,
    plus,
    minus,
    mul,
    div,
    pow,
    lparen,
    rparen,
    assign,
    semi,
    comma,
    id, // variable name
    keyword, // sqf command name
    end_of_file
};