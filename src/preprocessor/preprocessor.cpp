#include "src/preprocessor/preprocessor.h"
#include <sstream>
#include <regex>
#include <cctype>

Preprocessor::Preprocessor(std::istream &to_read) : stream_(to_read)
{
    // Immediately read in the first character (not an advance, don't want to change state)
    stream_.get(current_char_);
}

Preprocessor::Preprocessor(std::istream &to_read,
                           std::map<std::string, MacroArg> &params,
                           std::multimap<std::string, MacroDefinition> &defined,
                           std::unordered_set<std::string> &context) : stream_(to_read), macros_(defined), params_(params), macro_context_(context)
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

std::string Preprocessor::getWord()
{
    std::string word;
    while (std::isalnum(current_char_) || current_char_ == '_')
    {
        word.push_back(current_char_);
        advance();
    }

    return word;
}

std::vector<MacroArg> Preprocessor::getArgs(const std::string &word)
{
    int line = lineno_;
    int column = column_;

    std::vector<MacroArg> args;
    if (current_char_ == '(')
    {
        advance();

        // Position of each argument applied to replacements in full expansion
        MacroArg arg;
        arg.line = lineno_;
        arg.column = column_;

        int open_p = 1;
        while (current_char_ != '\0' && open_p != 0)
        {
            // Braces can be used inside arguments as long as the pairing is balanced
            if (current_char_ == '(')
            {
                open_p++;
            }
            else if (current_char_ == ')')
            {
                open_p--;
            }

            // Commas don't split arguments within nested braces
            if (current_char_ == ',' && open_p == 1)
            {
                args.push_back(arg);
                advance();
                arg.raw.clear();
                arg.line = lineno_;
                arg.column = column_;
            }
            else if (open_p != 0)
            {
                arg.raw.push_back(current_char_);
                advance();
            }
        }

        if (open_p != 0)
        {
            error(line, column, "Unclosed macro arguments '" + word + "('");
        }
        else
        {
            // Final argument ends on closing brace
            args.push_back(arg);
            advance();
        }
    }

    return args;
}

void Preprocessor::handleDirective()
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
        m.body = matches[3].str();

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

        macros_.insert({keyword, m});
    }
    else
    {
        // TODO improve errors
        error(lineno_, column_, "Invalid macro definition");
    }
}

// Obtains next word and arguments if present and expands to a macro in the buffer
void Preprocessor::processWord()
{
    // Position of the macro applied to all expanded characters
    int line = lineno_;
    int column = column_;

    // May just be object like
    std::string word = getWord();

    // May be function like
    std::vector<MacroArg> args;

    // Parameter replacement happens before macro expansion
    if (isParam(word))
    {
        appendToBuffer(params_.at(word).chars);
        return;
    }
    else if (isMacro(word) && !isRecursive(word))
    {
        args = getArgs(word);
    }
    else
    {
        // Just a normal word, push to buffer
        PosChar pc;
        pc.line = line;
        pc.column = column;
        for (char &c : word)
        {
            pc.c = c;
            peek_buffer_.push_back(pc);
            pc.column++;
        }

        return;
    }

    // Check if the macro is defined for that same number of args
    bool found = false;
    MacroDefinition macro_def;

    std::multimap<std::string, MacroDefinition>::iterator end = macros_.upper_bound(word);
    for (std::multimap<std::string, MacroDefinition>::iterator i = macros_.lower_bound(word); i != end; i++)
    {
        if (i->second.params.size() == args.size())
        {
            found = true;
            macro_def = i->second;
            break;
        };
    }

    // TODO improve error message with details (expected no. args)
    if (!found)
    {
        error(line, column, "Invalid number of macro arguments supplied '" + word + "'");
    }

    // Arguments are expanded before parameter replacement
    for (MacroArg &arg : args)
    {
        std::stringstream arg_ss(arg.raw);
        std::map<std::string, MacroArg> no_args;
        Preprocessor pp(arg_ss, no_args, macros_, macro_context_);
        arg.chars = pp.getAll();

        // Update reference position for all expanded argument characters
        for (PosChar &pc : arg.chars)
        {
            pc.line += arg.line - 1;
            pc.column += arg.column - 1;
        }
    }

    // Construct a map of parameter replacements
    std::map<std::string, MacroArg> param_map;
    for (size_t i = 0; i < args.size(); ++i)
        param_map[macro_def.params[i]] = args[i];

    // Prepare the set of macros to ignore in the body (prevent infinite recursion)
    std::unordered_set<std::string> body_context(macro_context_);
    body_context.insert(word);

    // Finally macro body is expanded and processed (done sequentially to preserve correct behaviour)
    std::stringstream body_ss(macro_def.body);
    Preprocessor pp(body_ss, param_map, macros_, body_context);
    appendToBuffer(pp.getAll());
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
            handleDirective();
            return get();
        }
        else if (macro_context_.size() > 0 && current_char_ == '#')
        {
            // Concatenate the previous and following tokens (i.e. skip these characters)
            if (stream_.peek() == '#')
            {
                advance();
                advance();
                return get();
            }
            // Stringize the following word token
            else
            {
                // Position will become macro position later
                PosChar quote;
                quote.c = '"';
                advance();

                processWord();
                peek_buffer_.push_back(quote);
                return quote;
            }
        }
        else if (std::isalpha(current_char_) || current_char_ == '_')
        {
            processWord();
            return get();
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

// Processes the whole input and returns the resulting sequence of positioned characters
std::vector<PosChar> Preprocessor::getAll()
{
    PosChar c = get();
    std::vector<PosChar> result;
    while (c != '\0')
    {
        result.push_back(c);
        c = get();
    }

    return result;
}
