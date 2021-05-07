#include "src/lexer/lexer.h"
#include "src/lexer/token_maps.h"
#include "src/sqf/keywords.h"
#include <string>
#include <array>
#include <istream>
#include <algorithm>
#include <cctype>
#include <iostream>

Lexer::Lexer(std::istream &to_read) : stream_(to_read)
{
    // Immediately read in the first character
    advance();
}

void Lexer::error()
{
    throw current_char_;
}

// Preview the next character in order to differentiate tokens that start the same
char Lexer::peek()
{
    return stream_.peek();
}

void Lexer::advance()
{
    // Increment the line whenever a newline is passed
    if (current_char_ == '\n')
    {
        lineno_++;
        column_ = 0;
    }

    stream_.get(current_char_);

    // When end of stream is reached return EOF character
    if (stream_.eof())
    {
        current_char_ = '\0';
    }
    else
    {
        column_++;
    }
}

void Lexer::skipWhitespace()
{
    while (current_char_ != '\0' && std::isspace(current_char_))
    {
        advance();
    }
}

void Lexer::skipComment()
{
    if (peek() == '/')
    {
        while (current_char_ != '\0' && current_char_ != '\n')
        {
            advance();
        }

        // Skip past the EOL
        advance();
    }
    else if (peek() == '*')
    {
        while (current_char_ != '\0' && !(current_char_ == '*' && peek() == '/'))
        {
            advance();
        }

        // Skip past the block end: */
        advance();
        advance();
    }
}

Token Lexer::_id()
{
    std::string result;
    while (current_char_ != '\0' && (std::isalnum(current_char_) || current_char_ == '_'))
    {
        result.push_back(current_char_);
        advance();
    }

    // SQF is not case sensitive
    for (auto &c : result)
    {
        c = std::tolower(c);
    }

    // Some keywords have specific token types to differentiate precedence
    if (SQF_Token_Keywords.find(result) != SQF_Token_Keywords.end())
    {
        return makeToken(SQF_Token_Keywords.at(result), result);
    }

    // SQF has a lot of reserved keywords
    // Differentiate nullarys for grammar clarity (see issue #11)
    if (SQF_Nullary_Keywords.find(result) != SQF_Nullary_Keywords.end())
    {
        return makeToken(TokenType::nullary, result);
    }
    else if (SQF_Keywords.find(result) != SQF_Keywords.end())
    {
        return makeToken(TokenType::keyword, result);
    }
    else
    {
        return makeToken(TokenType::id, result);
    }
}

Token Lexer::number()
{
    // First determine the type of numeric literal
    TokenType type = TokenType::dec_literal;
    if (current_char_ == '$')
    {
        type = TokenType::hex_literal;
        advance();
    }
    else if (current_char_ == '0' && peek() == 'x')
    {
        type = TokenType::hex_literal;
        advance();
        advance();
    }

    // Read the characters according to the literal type
    std::string value;
    if (type == TokenType::hex_literal)
    {
        while (current_char_ != '\0' && std::isxdigit(current_char_))
        {
            value.push_back(current_char_);
            advance();
        }
    }
    else
    {
        // Integer part (may not be present)
        while (current_char_ != '\0' && std::isdigit(current_char_))
        {
            value.push_back(current_char_);
            advance();
        }

        // Possible decimal point (can appear at the start or end)
        if (current_char_ == '.')
        {
            value.push_back(current_char_);
            advance();
        }

        // Possible fractional part
        while (current_char_ != '\0' && std::isdigit(current_char_))
        {
            value.push_back(current_char_);
            advance();
        }

        // Optional scientific notation suffix
        if (std::tolower(current_char_) == 'e') {
            value.push_back(current_char_);
            advance();

            // Can be followed by an optional + or -
            if (current_char_ == '+' || current_char_ == '-')
            {
                value.push_back(current_char_);
                advance();
            }

            // *Must* be followed by digits if present
            if (std::isdigit(current_char_))
            {
                while (current_char_ != '\0' && std::isdigit(current_char_))
                {
                    value.push_back(current_char_);
                    advance();
                }
            }
            else
            {
                std::cout << "unfinished scientific notation";
                error();
            }

        }
    }

    return makeToken(type, value);
}

Token Lexer::string()
{
    char enclosing = current_char_;
    int line = lineno_; // Token line at starting character pos
    std::string result;

    // Skip past beginning enclosing character
    advance();

    // Doubled enclosing character becomes single within string
    while (current_char_ != enclosing || peek() == enclosing)
    {
        // Unclosed string cannot be tokenised
        if (current_char_ == '\0')
        {
            std::cout << "unclosed string";
            error();
        }

        if (current_char_ == enclosing && peek() == enclosing)
        {
            advance();
        }

        result.push_back(current_char_);
        advance();
    }

    // Skip past end enclosing character
    advance();

    return makeToken(TokenType::str_literal, result);
}

// Convenience function to make a token with current lexer position
Token Lexer::makeToken(TokenType type, std::string raw)
{
    return Token(type, raw, lineno_, column_);
}

Token Lexer::nextToken()
{
    while (current_char_ != '\0')
    {
        // Whitespace is irrelevant in SQF
        if (std::isspace(current_char_))
        {
            skipWhitespace();
            continue;
        }

        // Comments are irrelevant (block and line)
        if (current_char_ == '/' && (peek() == '/' || peek() == '*'))
        {
            skipComment();
            continue;
        }

        if (std::isalpha(current_char_) || current_char_ == '_')
        {
            return _id();
        }

        // Multiple numeric literal formats
        if (
            std::isdigit(current_char_)
            // Decimal literals can start with the decimal point
            || (current_char_ == '.' && std::isdigit(peek()))
            // Hexadecimal literals can start with the dollar sign (or 0x)
            || (current_char_ == '$' && std::isxdigit(peek())))
        {
            return number();
        }

        if (current_char_ == '"' || current_char_ == '\'')
        {
            return string();
        }

        if (current_char_ == '=')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return makeToken(TokenType::eql, "==");
            }

            return makeToken(TokenType::assign, "=");
        }

        if (current_char_ == '!')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return makeToken(TokenType::neql, "!=");
            }

            return makeToken(TokenType::negation, "!");
        }

        if (current_char_ == '>')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return makeToken(TokenType::gteql, ">=");
            }
            if (current_char_ == '>')
            {
                advance();
                return makeToken(TokenType::gtgt, ">>");
            }

            return makeToken(TokenType::gt, ">");
        }

        if (current_char_ == '<')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return makeToken(TokenType::lteql, "<=");
            }

            return makeToken(TokenType::lt, "<");
        }

        if (current_char_ == '|' && peek() == '|')
        {
            advance();
            advance();
            return makeToken(TokenType::disjunction, "||");
        }

        if (current_char_ == '&' && peek() == '&')
        {
            advance();
            advance();
            return makeToken(TokenType::conjunction, "&&");
        }

        // General handling of single character tokens
        if (SQF_Token_Chars.find(current_char_) != SQF_Token_Chars.end())
        {
            std::string raw;
            raw.push_back(current_char_);
            Token t = makeToken(SQF_Token_Chars.at(current_char_), raw);

            advance();

            return t;
        }

        std::cout << "unexpected character: " << current_char_;
        error();
    }

    return makeToken(TokenType::end_of_file, "");
}