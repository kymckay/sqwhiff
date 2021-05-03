#include "src/lexer/lexer.h"
#include "src/tokens/token.h"
#include "src/tokens/keywords.h"
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
    stream_.get(current_char_);

    // When end of stream is reached return EOF character
    if (stream_.eof())
    {
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
    for (auto &c : result)
    {
        c = std::tolower(c);
    }

    if (result == "private")
    {
        return Token(TokenType::private_op, result, line_);
    }

    if (result == "mod")
    {
        return Token(TokenType::mod, result, line_);
    }

    if (result == "atan2")
    {
        return Token(TokenType::atan2, result, line_);
    }

    if (result == "min")
    {
        return Token(TokenType::min, result, line_);
    }

    if (result == "max")
    {
        return Token(TokenType::max, result, line_);
    }

    if (result == "not")
    {
        return Token(TokenType::negation, result, line_);
    }

    if (result == "or")
    {
        return Token(TokenType::disjunction, result, line_);
    }

    if (result == "and")
    {
        return Token(TokenType::conjunction, result, line_);
    }

    if (result == "else")
    {
        return Token(TokenType::else_op, result, line_);
    }

    // SQF has a lot of reserved keywords
    // Differentiate nullarys for grammar clarity (see issue #11)
    if (SQF_Nullary_Keywords.find(result) != SQF_Nullary_Keywords.end())
    {
        return Token(TokenType::nullary, result, line_);
    }
    else if (SQF_Keywords.find(result) != SQF_Keywords.end())
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
                std::cout << "unfinished scientific notation";
                error();
            }

        }
    }

    return Token(type, value, line_);
}

Token Lexer::string()
{
    char enclosing = current_char_;
    int line = line_; // Token line at starting character pos
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

    return Token(TokenType::str_literal, result, line);
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
                return Token(TokenType::eql, "==", line_);
            }

            return Token(TokenType::assign, "=", line_);
        }

        if (current_char_ == '!')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return Token(TokenType::neql, "!=", line_);
            }

            return Token(TokenType::negation, "!", line_);
        }

        if (current_char_ == '>')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return Token(TokenType::gteql, ">=", line_);
            }
            if (current_char_ == '>')
            {
                advance();
                return Token(TokenType::gtgt, ">>", line_);
            }

            return Token(TokenType::gt, ">", line_);
        }

        if (current_char_ == '<')
        {
            advance();
            if (current_char_ == '=')
            {
                advance();
                return Token(TokenType::lteql, "<=", line_);
            }

            return Token(TokenType::lt, "<", line_);
        }

        if (current_char_ == '|' && peek() == '|')
        {
            advance();
            advance();
            return Token(TokenType::disjunction, "||", line_);
        }

        if (current_char_ == '&' && peek() == '&')
        {
            advance();
            advance();
            return Token(TokenType::conjunction, "&&", line_);
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

        if (current_char_ == '#')
        {
            advance();
            return Token(TokenType::hash, "#", line_);
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

        if (current_char_ == '[')
        {
            advance();
            return Token(TokenType::lsqb, "[", line_);
        }

        if (current_char_ == ']')
        {
            advance();
            return Token(TokenType::rsqb, "]", line_);
        }

        if (current_char_ == '{')
        {
            advance();
            return Token(TokenType::lcurl, "{", line_);
        }

        if (current_char_ == '}')
        {
            advance();
            return Token(TokenType::rcurl, "}", line_);
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

        std::cout << "unexpected character: " << current_char_;
        error();
    }

    return Token(TokenType::end_of_file, "", line_);
}