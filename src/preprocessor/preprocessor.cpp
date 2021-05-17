#include "src/preprocessor/preprocessor.h"
#include <string>
#include <istream>
#include <sstream>
#include <regex>
#include <cctype>

Preprocessor::Preprocessor(std::istream &to_read) : stream_(to_read)
{
    // Immediately read in the first character (not an advance, don't want to change state)
    stream_.get(current_char_);
}

void Preprocessor::error(int line, int col, std::string msg)
{
    throw PreprocessingError(line, col, msg);
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
        while (current_char_ != '\0' && !(current_char_ == '*' && stream_.peek() == '/'))
        {
            advance();
        }

        // Skip past the block end: */
        advance();
        advance();
    }
}

PosChar Preprocessor::handleDirective()
{
    // Directive position important for errors and macros
    int line = lineno_;
    int col = column_;

    // Skip the #
    advance();

    std::string directive;
    while (current_char_ != '\n' && current_char_ != '\0')
    {
        // Logical line can be extended by escaped newlines (anywhere in the directive)
        if (current_char_ == '\\' && stream_.peek() == '\n')
        {
            advance();
            advance();
        }
        else
        {
            directive.push_back(current_char_);
            advance();
        }
    }

    // There can be no space between the # and directive
    // Space characters between the instruction and body are skipped
    std::regex rgx(R"((\w+) *(.*))");
    std::smatch matches;

    if (std::regex_match(directive, matches, rgx))
    {
        // 0 is the whole match
        std::string instruction = matches[1].str();
        std::string body = matches[2].str();

        if (instruction == "include")
        {
        }
        else if (instruction == "define")
        {
            handleDefine(body);
        }
        else if (instruction == "undef")
        {
        }
        else if (instruction == "if")
        {
        }
        else if (instruction == "ifdef")
        {
        }
        else if (instruction == "ifndef")
        {
        }
        else if (instruction == "else")
        {
        }
        else if (instruction == "endif")
        {
        }
        else
        {
            error(line, col, "Unrecognised preprocessor directive '#" + instruction + "'");
        }
    }
    else
    {
        error(line, col, "Malformed preprocessor directive");
    }

    return get();
}

void Preprocessor::handleDefine(const std::string &body)
{
    // ID must start with alpha or underscore, can contain digits
    // Arguments are optional, trailing comma is allowed
    // Space characters (specifically spaces) following the head are skipped
    // Body may be empty
    std::regex rgx(R"(([a-zA-Z_][0-9a-zA-Z_]*)(?:\(((?:[a-zA-Z_][0-9a-zA-Z_]*,?)+)\))? *(.*))");
    std::smatch matches;

    if (std::regex_match(body, matches, rgx))
    {
        std::string keyword = matches[1].str();
        std::stringstream argstream(matches[2].str());

        Macro m;
        m.body = matches[3].str();

        // Populate args vector
        std::string tmp;
        while (std::getline(argstream, tmp, ','))
        {
            // TODO trim args of horizontal whitespace
            m.args.push_back(tmp);
        }

        macros_.insert({keyword, m});
    }
    else
    {
        // TODO improve errors
        error(lineno_, column_, "Invalid macro definition");
    }
}

PosChar Preprocessor::get()
{
    // Pull from the buffer first if any peek has occured
    if (!peek_buffer_.empty())
    {
        PosChar p = peek_buffer_.front();
        peek_buffer_.pop_front();
        return p;
    }

    // Preprocessing does not occur within double quoted string literals
    if (!in_doubles_)
    {
        // Comments are irrelevant (block and line)
        if (current_char_ == '/' && (stream_.peek() == '/' || stream_.peek() == '*'))
        {
            skipComment();
        }
        // Preprocessor directives indicated by # at line start
        else if (line_start_ && current_char_ == '#')
        {
            return handleDirective();
        }
        // TODO: Recognise macro usage, if previous character wasn't alpha or _ then could be a macro
        else if (std::isalpha(current_char_) || current_char_ == '_')
        {}
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

// Allows looking ahead to future characters in order to differentiate tokens that start the same
PosChar Preprocessor::peek(int peek_by)
{
    while (peek_buffer_.size() < peek_by)
    {
        peek_buffer_.push_back(get());
    }

    // Convert peek request to 0-indexed
    return peek_buffer_.at(peek_by - 1);
}