#include "src/preprocessor/preprocessor.h"
#include <string>
#include <istream>
#include <algorithm>
#include <cctype>

Preprocessor::Preprocessor(std::istream &to_read) : stream_(to_read)
{
    // Immediately read in the first character
    advance();
}

void Preprocessor::error(PosChar p, std::string msg)
{
    throw PreprocessingError(p.line, p.column, msg);
}

void Preprocessor::advance()
{
    // Increment the line whenever a newline is passed
    if (current_char_ == '\n')
    {
        lineno_++;
        column_ = 0;
        line_start_ = true;
    }
    else if (line_start_ && !std::isspace(current_char_))
    {
        line_start_ = false;
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

void Preprocessor::skipComment()
{
    if (stream_.peek() == '/')
    {
        // Intentionally don't skip the newline at the end (acts as a delimiter)
        while (current_char_ != '\0' && current_char_ != '\n')
        {
            advance();
        }
    }
    else if (stream_.peek() == '*')
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

PosChar Preprocessor::get()
{
    // Preprocessing does not occur within double quoted string literals
    if (!in_doubles_)
    {
        // Comments are irrelevant (block and line)
        if (current_char_ == '/' && (stream_.peek() == '/' || stream_.peek() == '*'))
        {
            skipComment();
        }
    }

    // Each double quote encountered inverts the context
    if (current_char_ == '"')
    {
        in_doubles_ = !in_doubles_;
    }

    PosChar c;
    c.line = lineno_;
    c.column = column_;
    c.c = current_char_;

    // Remember to actually progress through the input
    advance();

    return c;
}

// Preview the next character in order to differentiate tokens that start the same
char Preprocessor::peek()
{
    return stream_.peek();
}