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
    std::regex rgx(R"(([a-zA-Z]+) *(.*))");
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
            defineMacro(body);
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
        error(line, col, "Invalid preprocessor directive");
    }

    return get();
}

void Preprocessor::defineMacro(const std::string &definition)
{
    // ID must start with alpha or underscore, can contain digits
    // Arguments are optional, trailing comma is allowed
    // Space characters (specifically spaces) following the head are skipped
    // Body may be empty
    std::regex rgx(R"(([a-zA-Z_][0-9a-zA-Z_]*)(?:\(((?:[a-zA-Z_][0-9a-zA-Z_]*,?)+)\))? *(.*))");
    std::smatch matches;

    if (std::regex_match(definition, matches, rgx))
    {
        std::string keyword = matches[1].str();
        std::stringstream argstream(matches[2].str());

        Macro m;

        // Populate args vector
        std::string tmp;
        while (std::getline(argstream, tmp, ','))
        {
            // TODO trim args of horizontal whitespace (do in the regex)
            m.args.push_back(tmp);
        }

        // Split body by concatenation makes lifes simplier when resolving macros later
        std::string body = matches[3].str();
        while (body.length() > 0)
        {
            std::string::size_type i = body.find("##");
            if (i == std::string::npos)
            {
                m.body.push_back(body);
                body.clear();
            }
            else
            {
                m.body.push_back(body.substr(0, i));
                body = body.substr(i + 2);
            }
        }

        macros_.insert({keyword, m});
    }
    else
    {
        // TODO improve errors
        error(lineno_, column_, "Invalid macro definition");
    }
}

PosChar Preprocessor::expandMacro()
{
    PosChar initial;
    initial.c = current_char_;
    initial.line = lineno_;
    initial.column = column_;

    // TODO Need to use preprocessor get, since macros passed in to arguments are resolved first
    std::vector<PosChar> word_peek;
    std::string word;
    while (std::isalpha(current_char_) || current_char_ == '_')
    {
        word.push_back(current_char_);

        // This is a form of peeking, be prepared to place to buffer if not a macro
        PosChar c;
        c.c = current_char_;
        c.line = lineno_;
        c.column = column_;
        word_peek.push_back(c);

        advance();
    }

    if (macros_.find(word) != macros_.end())
    {
        // Any arguments must follow immediately
        std::vector<std::string> args;
        if (current_char_ == '(')
        {
            advance();

            std::string arg;
            while (current_char_ != '\0' && current_char_ != ')')
            {
                if (current_char_ == ',')
                {
                    args.push_back(arg);
                    arg.clear();
                }
                else
                {
                    arg.push_back(current_char_);
                }

                advance();
            }
            // The final argument ends on a )
            args.push_back(arg);

            if (current_char_ == ')')
            {
                advance();
            }
            else
            {
                error(initial.line, initial.column, "Unclosed macro arguments '" + word + "('");
            }
        }

        // With the args known, check if the macro is defined for that same number of args
        bool found = false;
        Macro matched;

        std::multimap<std::string, Macro>::iterator end = macros_.upper_bound(word);
        for (std::multimap<std::string, Macro>::iterator i = macros_.lower_bound(word); i != end; i++)
        {
            if (i->second.args.size() == args.size())
            {
                found = true;
                matched = i->second;
                break;
            };
        }

        if (!found)
        {
            // TODO improve error message with details
            error(initial.line, initial.column, "Invalid number of macro arguments supplied '" + word + "'");
        }

        std::string body;
        for (std::string body_part : matched.body)
        {
            for (size_t i = 0; i < args.size(); i++)
            {
                std::string param = matched.args[i];
                std::string arg = args[i];

                // Stringify argument resolutions where appropriate
                std::regex rgx_stringify("#" + param + "\\b");
                body_part = regex_replace(body_part, rgx_stringify, "\"" + arg + "\"");

                // Resolve plain arguments
                std::regex rgx_param("\\b" + param + "\\b");
                body_part = regex_replace(body_part, rgx_param, arg);
            }

            // Resolve static stringification
            std::regex rgx_stringify("#([a-zA-Z_]\\w*)");
            body_part = regex_replace(body_part, rgx_stringify, "\"$1\"");

            body.append(body_part);
        }

        std::vector<PosChar> resolved;
        for (auto &&c : body)
        {
            // Report errors at the macro position
            PosChar pc = initial;
            pc.c = c;
            resolved.push_back(pc);
        }

        // This may not be most efficient if buffer is empty already, but should never be too long anyway
        peek_buffer_.insert(peek_buffer_.end(), resolved.begin(), resolved.end());
    }
    else
    {
        // This may not be most efficient if buffer is empty already, but should never be too long anyway
        peek_buffer_.insert(peek_buffer_.end(), word_peek.begin(), word_peek.end());
    }

    // Either a macro was resolved or word pushed to buffer, get as normal
    return get();
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
        else if (std::isalpha(current_char_) || current_char_ == '_')
        {
            return expandMacro();
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