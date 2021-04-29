#include "lexer.h"
#include "token.h"
#include "keywords.h"
#include <string>
#include <array>
#include <fstream>
#include <algorithm>
#include <cctype>

// Open the file resource immediately for reading
// A lexer should never be instantiated unless the intention is to read the file so this is expected
Lexer::Lexer(std::ifstream &file) : file_(file)
{
    // Read in the first character
    if (file_.is_open())
        advance();
}

// Close the file resource upon destruction
Lexer::~Lexer()
{
    if (file_.is_open())
        file_.close();
}

void Lexer::error()
{
    throw "Unexpected character: " + current_char_;
}

// Preview the next character in order to differentiate tokens that start the same
char Lexer::peek()
{
    return file_.peek();
}

void Lexer::advance()
{
    file_.get(current_char_);

    // Be sure to close the file when done with it
    if (file_.eof())
    {
        file_.close();
        current_char_ = '\0';
    }

    // Increment the line whenever a newline is found
    if (current_char_ == '\n')
        line_++;
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
    for (auto c : result)
    {
        std::tolower(c);
    }

    if (result == "mod")
    {
        return Token(TokenType::mod, result, line_);
    }

    if (result == "atan2")
    {
        return Token(TokenType::atan2, result, line_);
    }

    // SQF has a lot of reserved keywords
    if (SQF_Keywords.find(result) != SQF_Keywords.end())
    {
        return Token(TokenType::keyword, result, line_);
    }
    else
    {
        return Token(TokenType::id, result, line_);
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
                error();
            }

        }
    }

    return Token(type, value, line_);
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

        if (current_char_ == '+')
        {
            advance();
            return Token(TokenType::plus, "+", line_);
        }

        if (current_char_ == '-')
        {
            advance();
            return Token(TokenType::minus, "-", line_);
        }

        if (current_char_ == '*')
        {
            advance();
            return Token(TokenType::mul, "*", line_);
        }

        if (current_char_ == '/')
        {
            advance();
            return Token(TokenType::div, "/", line_);
        }

        if (current_char_ == '^')
        {
            advance();
            return Token(TokenType::pow, "^", line_);
        }

        if (current_char_ == '%')
        {
            advance();
            return Token(TokenType::mod, "%", line_);
        }

        if (current_char_ == '(')
        {
            advance();
            return Token(TokenType::lparen, "(", line_);
        }

        if (current_char_ == ')')
        {
            advance();
            return Token(TokenType::rparen, ")", line_);
        }

        if (current_char_ == '=')
        {
            advance();
            return Token(TokenType::assign, "=", line_);
        }

        if (current_char_ == ';')
        {
            advance();
            return Token(TokenType::semi, ";", line_);
        }

        if (current_char_ == ',')
        {
            advance();
            return Token(TokenType::comma, ",", line_);
        }

        error();
    }

    return Token(TokenType::end_of_file, "", line_);
}