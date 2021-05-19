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

    void error(int, int, std::string);
    void advance();
    void skipComment();

    // Multimap since macros can be overloaded, ordered allows iteration over the subsets
    std::multimap<std::string, MacroDefinition> macros_;

    PosChar handleDirective();
    void defineMacro(const std::string &);
    std::vector<PosChar> expandMacro(const MacroToken&);
    void getMacroArgs(MacroToken&);
    PosChar expandMacro();

public:
    Preprocessor(std::istream&);
    PosChar get();
    PosChar peek(int = 1);
};