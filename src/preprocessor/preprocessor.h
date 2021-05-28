#pragma once
#include "src/preprocessor/pos_char.h"
#include "src/preprocessor/macro.h"
#include "src/preprocessor/preprocessing_error.h"
#include <string>
#include <istream>
#include <deque>
#include <unordered_map>
#include <unordered_set>

class Preprocessor
{
    // Reference member for polymorphism
    std::istream &stream_;

    // Use buffer to simplify lookahead logic with preprocessing
    std::deque<PosChar> peek_buffer_;

    inline void appendToBuffer(const std::vector<PosChar> &chars)
    {
        peek_buffer_.insert(peek_buffer_.end(), chars.begin(), chars.end());
    }

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

    // Multimap since macros can be overloaded, ordered allows iteration over the subsets
    std::unordered_map<std::string, MacroDefinition> macros_;

    // When used in subcontext parameter replacement may take place
    std::unordered_map<std::string, MacroArg> params_;

    // Set of macros that aren't expanded in the current context to prevent recursion
    std::unordered_set<std::string> macro_context_;

    inline bool isMacro(const std::string &word)
    {
        return macros_.find(word) != macros_.end();
    }

    inline bool isParam(const std::string &word)
    {
        return params_.find(word) != params_.end();
    }

    inline bool isRecursive(const std::string &word)
    {
        return macro_context_.find(word) != macro_context_.end();
    }

    // Class recursively used to expand nested macro usage contexts
    Preprocessor(std::istream &,
                 std::unordered_map<std::string, MacroArg> &,        // Parameter map for replacement (only relevant to macro body expansion)
                 std::unordered_map<std::string, MacroDefinition> &, // Set of defined macros for expansion (nested)
                 std::unordered_set<std::string> &                   // Set of macros not to expand (prevents macro recursion)
    );

    std::string getWord();
    std::vector<MacroArg> getArgs(const std::string &);

    void handleDirective();
    void defineMacro(const std::string &);
    void processWord();

public:
    Preprocessor(std::istream &);
    PosChar get();
    PosChar peek(int = 1);
    std::vector<PosChar> getAll();
};