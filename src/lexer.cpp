#include "lexer.h"
#include "token.h"
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

    // TODO check for keywords
    if (false)
    {
        return Token();
    }
    else
    {
        return Token(TokenType::id, result, line_);
    }
}

// TODO Handle decimals and scientific notation
std::string Lexer::number()
{
    std::string result;
    while (current_char_ != '\0' && std::isdigit(current_char_))
    {
        result.push_back(current_char_);
        advance();
    }
    return result;
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

        if (std::isdigit(current_char_))
            return Token(TokenType::number, number(), line_);

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