#include "lexer.h"
#include "token.h"
#include <string>
#include <array>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>

// All possible SQF token delimiting characters (some of these form parts of larger operators, that structuring is handled by parser)
const std::array<char, NUM_DELIMITERS> Lexer::delimiters_ = {'\\', '>', '|', '\"', '\'', ' ', '\t', '\r', '\n', '=', ':', '{', '}', '(', ')', '[', ']', ';', ',', '!', '/', '*', '+', '-', '%', '^', '>', '<'};

// Open the file resource immediately for reading
// A lexer should never be instantiated unless the intention is to read the file so this is expected
Lexer::Lexer(std::string file)
{
    file_ = std::ifstream(file);
}

// Close the file resource upon destruction
Lexer::~Lexer()
{
    if (file_.is_open())
        file_.close();
}

Token Lexer::nextToken()
{
    Token base = nextRawToken();

    if (base.raw == "+")
        base.type = TokenType::plus;
    else if (base.raw == "-")
        base.type = TokenType::minus;
    else if (base.raw == "/")
        base.type = TokenType::div;
    else if (base.raw == "*")
        base.type = TokenType::mul;
    else if (base.raw.empty())
        base.type = TokenType::end_of_file;
    else
        base.type = TokenType::number;

    return base;
}

// Get the next token, reads through the file until a delimiter is found or EOF is hit
Token Lexer::nextRawToken()
{
    // Delimiter used to produce previous token is itself a token
    if (delimiter_ != nullptr)
    {
        Token toke = Token(std::string(delimiter_), line_);
        delimiter_ = nullptr;
        return toke;
    }

    std::string text = "";
    if (file_.is_open())
    {

        while (file_.get(current_char_))
        {
            // Increment the line whenever a newline is found
            if (current_char_ == '\n')
                line_++;

            if (isDelimiter(current_char_))
            {
                if (text.empty())
                {
                    return Token(std::string(&current_char_), line_);
                }
                else
                {
                    delimiter_ = &current_char_;
                    return Token(text, line_);
                }
            }

            text.push_back(current_char_);
        }

        // EOF was reached
        file_.close();
    }

    // EOF reached
    return Token(text, line_);
}

bool Lexer::isDelimiter(const char c) {
    return std::find(delimiters_.begin(), delimiters_.end(), c) != delimiters_.end();
}

int main()
{
    Lexer test("test.txt");
    Token t;
    do
    {
        t = test.nextToken();
        switch (t.type)
        {
        case TokenType::plus:
            std::cout << "plus ";
            break;
        case TokenType::minus:
            std::cout << "minus ";
            break;
        case TokenType::number:
            std::cout << "number ";
            break;
        case TokenType::end_of_file:
            std::cout << "eof ";
            break;
        default:
            std::cout << "unknown ";
            break;
        }
        std::cout << t.line << ": " << t.raw << "\n";
    } while (t.type != TokenType::end_of_file);
}