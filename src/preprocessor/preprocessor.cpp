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

Preprocessor::Preprocessor(std::istream &to_read, std::multimap<std::string, MacroDefinition> &defined) : stream_(to_read), macros_(defined)
{
    // Preprocessor is being used recursively to expand macro bodies and arguments
    expand_only_ = true;

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
    // Parameters are optional, trailing comma is allowed
    // Space characters (specifically spaces) following the head are skipped
    // Body may be empty
    std::regex rgx(R"(([a-zA-Z_][0-9a-zA-Z_]*)(?:\(((?:\s*[a-zA-Z_][0-9a-zA-Z_]*\s*,?)+)\s*\))? *(.*))");
    std::smatch matches;

    if (std::regex_match(definition, matches, rgx))
    {
        std::string keyword = matches[1].str();
        std::string params = matches[2].str();

        MacroDefinition m;

        // Populate parameters vector
        std::string tmp;
        for (char &c : params)
        {
            if (c == ',')
            {
                m.params.push_back(tmp);
                tmp.clear();
            }
            // Horizontal whitespace in parameters is ignored
            else if (!std::isspace(c))
            {
                tmp.push_back(c);
            }
        }

        // Final param may not end with a comma
        if (tmp.length() > 0)
        {
            m.params.push_back(tmp);
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

// Expands a macro from the map with nesting
void Preprocessor::expandMacro(MacroToken &macro)
{
    // With the args known, check if the macro is defined for that same number of args
    bool found = false;
    MacroDefinition matched;

    std::multimap<std::string, MacroDefinition>::iterator end = macros_.upper_bound(macro.word);
    for (std::multimap<std::string, MacroDefinition>::iterator i = macros_.lower_bound(macro.word); i != end; i++)
    {
        if (i->second.params.size() == macro.args.size())
        {
            found = true;
            matched = i->second;
            break;
        };
    }

    if (!found)
    {
        // TODO improve error message with details
        error(macro.line, macro.column, "Invalid number of macro arguments supplied '" + macro.word + "'");
    }

    std::string body;
    for (std::string body_part : matched.body)
    {
        for (size_t i = 0; i < macro.args.size(); i++)
        {
            std::string param = matched.params[i];
            std::string arg = macro.args[i];

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

    for (auto &&c : body)
    {
        // Report errors at the macro position
        PosChar pc;
        pc.line = macro.line;
        pc.column = macro.column;
        pc.c = c;
        macro.expanded.push_back(pc);
    }
}

// Splits up the arguments string and handles preprocessing within
void Preprocessor::processMacroArgs(MacroToken &macro)
{
    int open_paren = 0;
    std::string arg;
    std::vector<std::string> args;
    for (char &c : macro.raw_args)
    {
        // Commas in nested parentheses don't seperate args
        if (c == ',' && open_paren == 0)
        {


            args.push_back(arg);
            arg.clear();
        }
        else
        {
            if (c == '(')
            {
                open_paren++;
            }
            else if (c == ')')
            {
                open_paren--;
            }

            arg.push_back(c);
        }
    }

    // Last argument doesn't end on a comma, may be a trailing comma
    if (arg.length() > 0)
    {
        args.push_back(arg);
    }

    for (std::string &s : args)
    {
        std::stringstream ss(s);
        Preprocessor pp(ss, macros_);

        std::string processed;
        char c = pp.get();
        while (c != '\0')
        {
            processed.push_back(c);
            c = pp.get();
        }

        macro.args.push_back(processed);
    }
}

// Read arguments from the current file position into the macro (if there are any)
void Preprocessor::getMacroArgs(MacroToken &macro)
{
    if (current_char_ == '(')
    {
        advance();

        int open_paren = 1;
        while (current_char_ != '\0')
        {
            // Parentheses can be used inside arguments as long as the pairing is consistent
            if (current_char_ == '(')
            {
                open_paren++;
            }
            else if (current_char_ == ')')
            {
                open_paren--;
            }

            // Arguments closed
            if (open_paren == 0)
            {
                advance();
                return;
            }

            // Everything is consumed, split later
            macro.raw_args.push_back(current_char_);
            advance();
        }

        // If while loop completes the EOF was reached
        error(macro.line, macro.column, "Unclosed macro arguments '" + macro.word + "('");
    }
}

// Obtains next word and expands if it's a macro
PosChar Preprocessor::processWord()
{
    MacroToken macro;
    macro.line = lineno_;
    macro.column = column_;

    std::vector<PosChar> word_peek;
    while (std::isalnum(current_char_) || current_char_ == '_')
    {
        macro.word.push_back(current_char_);

        // This is a form of peeking, be prepared to place to buffer if not a macro
        PosChar c;
        c.c = current_char_;
        c.line = lineno_;
        c.column = column_;
        word_peek.push_back(c);

        advance();
    }

    if (isMacro(macro.word))
    {
        // Any arguments must follow immediately
        getMacroArgs(macro);
        processMacroArgs(macro);

        // Handles any nested expansion
        expandMacro(macro);

        appendToBuffer(macro.expanded);
    }
    else
    {
        // Just a regular token
        appendToBuffer(word_peek);
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
        if (!expand_only_ && current_char_ == '/' && (stream_.peek() == '/' || stream_.peek() == '*'))
        {
            skipComment();
        }
        // Preprocessor directives indicated by # at line start
        else if (!expand_only_ && line_start_ && current_char_ == '#')
        {
            return handleDirective();
        }
        else if (std::isalpha(current_char_) || current_char_ == '_')
        {
            return processWord();
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