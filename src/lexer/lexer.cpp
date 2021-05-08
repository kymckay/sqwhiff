#include "src/lexer/lexer.h"
#include "src/lexer/token_maps.h"
#include "src/errors/error_messages.h"
#include "src/sqf/keywords.h"
#include <string>
#include <array>
#include <istream>
#include <ostream>
#include <algorithm>
#include <cctype>
#include <exception>

Lexer::Lexer(std::istream &to_read) : stream_(to_read)
{
    // Immediately read in the first character
    advance();
}

void Lexer::error(Token t, ErrorType type)
{
    Error e;
    e.token = t;
    e.type = type;
    errors_.push_back(e);
    throw std::runtime_error(ErrorMessages.at(e.type));
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
    // Token position at first character
    Token t = makeToken(TokenType::unknown, "");

    while (current_char_ != '\0' && (std::isalnum(current_char_) || current_char_ == '_'))
    {
        // SQF is not case sensitive
        t.raw.push_back(std::tolower(current_char_));
        advance();
    }

    // Some keywords have specific token types to differentiate precedence
    if (SQF_Token_Keywords.find(t.raw) != SQF_Token_Keywords.end())
    {
        t.type = SQF_Token_Keywords.at(t.raw);
    }
    // Differentiate nullarys for grammar clarity (see issue #11)
    else if (SQF_Nullary_Keywords.find(t.raw) != SQF_Nullary_Keywords.end())
    {
        t.type = TokenType::nullary;
    }
    // Remaining keywords may be unary or binary, parser differentiates these
    else if (SQF_Keywords.find(t.raw) != SQF_Keywords.end())
    {
        t.type = TokenType::keyword;
    }
    else
    {
        t.type = TokenType::id;
    }

    return t;
}

Token Lexer::number()
{
    Token t = makeToken(TokenType::dec_literal, "");

    // First determine the type of numeric literal
    if (current_char_ == '$')
    {
        t.type = TokenType::hex_literal;
        advance();
    }
    else if (current_char_ == '0' && peek() == 'x')
    {
        t.type = TokenType::hex_literal;
        advance();
        advance();
    }

    // Read the characters according to the literal type
    if (t.type == TokenType::hex_literal)
    {
        while (current_char_ != '\0' && std::isxdigit(current_char_))
        {
            t.raw.push_back(current_char_);
            advance();
        }
    }
    else
    {
        // Integer part (may not be present)
        while (current_char_ != '\0' && std::isdigit(current_char_))
        {
            t.raw.push_back(current_char_);
            advance();
        }

        // Possible decimal point (can appear at the start or end)
        if (current_char_ == '.')
        {
            t.raw.push_back(current_char_);
            advance();
        }

        // Possible fractional part
        while (current_char_ != '\0' && std::isdigit(current_char_))
        {
            t.raw.push_back(current_char_);
            advance();
        }

        // Optional scientific notation suffix
        if (std::tolower(current_char_) == 'e') {
            t.raw.push_back(current_char_);
            advance();

            // Can be followed by an optional + or -
            if (current_char_ == '+' || current_char_ == '-')
            {
                t.raw.push_back(current_char_);
                advance();
            }

            // *Must* be followed by digits if present
            if (std::isdigit(current_char_))
            {
                while (current_char_ != '\0' && std::isdigit(current_char_))
                {
                    t.raw.push_back(current_char_);
                    advance();
                }
            }
            else
            {
                error(t, ErrorType::incomplete_sci);
            }
        }
    }

    return t;
}

Token Lexer::string()
{
    char enclosing = current_char_;
    Token t = makeToken(TokenType::unknown, "");

    // Skip past beginning enclosing character
    advance();

    // Doubled enclosing character becomes single within string
    while (current_char_ != enclosing || peek() == enclosing)
    {
        // Unclosed string cannot be tokenised
        if (current_char_ == '\0')
        {
            error(t, ErrorType::unclosed_string);
        }

        if (current_char_ == enclosing && peek() == enclosing)
        {
            advance();
        }

        t.raw.push_back(current_char_);
        advance();
    }

    // Skip past end enclosing character
    advance();

    t.type = TokenType::str_literal;
    return t;
}

// Convenience function to make a token with current lexer position
Token Lexer::makeToken(TokenType type, std::string raw)
{
    Token t;
    t.type = type;
    t.raw = raw;
    t.line = lineno_;
    t.column = column_;
    return t;
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
            Token t = makeToken(SQF_Token_Chars.at(current_char_), std::string(1, current_char_));

            advance();

            return t;
        }

        // TODO: report the unexpected char
        error(makeToken(TokenType::unknown, std::string(1, current_char_)), ErrorType::unexpected_character);
    }

    return makeToken(TokenType::end_of_file, "");
}

void Lexer::logErrors(std::ostream &out)
{
    for (auto &&e : errors_)
    {
        out << e.token.line << ":" << e.token.column << " " << ErrorMessages.at(e.type);
    }
}