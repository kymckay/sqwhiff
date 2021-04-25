#include "lexer.h"
#include "token.h"
#include <string>
#include <array>
#include <fstream>
#include <algorithm>
#include <cctype>

// All possible SQF token delimiting characters (some of these form parts of larger operators, that structuring is handled by parser)
const std::array<char, NUM_DELIMITERS> Lexer::delimiters_ = {'\\', '>', '|', '\"', '\'', ' ', '\t', '\r', '\n', '=', ':', '{', '}', '(', ')', '[', ']', ';', ',', '!', '/', '*', '+', '-', '%', '^', '>', '<'};

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

bool Lexer::isDelimiter(const char c)
{
    return std::find(delimiters_.begin(), delimiters_.end(), c) != delimiters_.end();
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

        error();
    }

    return Token(TokenType::end_of_file, "", line_);
}