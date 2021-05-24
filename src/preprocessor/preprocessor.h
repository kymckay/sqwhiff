#pragma once
#include "src/preprocessor/pos_char.h"
#include "src/preprocessor/macro.h"
#include "src/preprocessor/preprocessing_error.h"
#include <string>
#include <istream>
#include <deque>
#include <map>

class Preprocessor
{
    // Reference member for polymorphism
    std::istream &stream_;

    // Use buffer to simplify lookahead logic with preprocessing
    std::deque<PosChar> peek_buffer_;

    char current_char_ = '\0';
    // Current physical position preprocessor has reached
    // Used to give a position to errors and macros
    int lineno_ = 1;
    int column_ = 1;

    // Preprocessor directives must appear at a line start (ignoring whitespace)
    bool line_start_ = true;

    // Double quoted string literals are not preprocessed
    bool in_doubles_ = false;

    // May be used recursively to expand nested macros and arguments
    bool expand_only_ = false;

    void error(int, int, std::string);
    void advance();
    void skipComment();

    inline void appendToBuffer(const std::vector<PosChar> &chars)
    {
        peek_buffer_.insert(peek_buffer_.end(), chars.begin(), chars.end());
    }

    // Multimap since macros can be overloaded, ordered allows iteration over the subsets
    std::multimap<std::string, MacroDefinition> macros_;

    inline bool isMacro(const std::string &word)
    {
        return macros_.find(word) != macros_.end();
    }

    Preprocessor(std::istream &, std::multimap<std::string, MacroDefinition>&);

    PosChar handleDirective();
    void defineMacro(const std::string &);
    void expandMacro(MacroToken &);
    void processMacroArgs(MacroToken &);
    void getMacroArgs(MacroToken&);
    PosChar processWord();

public:
    Preprocessor(std::istream &);
    PosChar get();
    PosChar peek(int = 1);
    std::string processAll();
};