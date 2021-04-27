#pragma once

enum class TokenType
{
    unknown,
    number,
    plus,
    minus,
    mul,
    div,
    lparen,
    rparen,
    assign,
    semi,
    comma,
    id, // variable name
    end_of_file
};