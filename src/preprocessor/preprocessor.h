#pragma once
#include <deque>
#include <filesystem>
#include <istream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "./macro.h"
#include "./pos_char.h"
#include "./preprocessing_error.h"

namespace fs = std::filesystem;

class Preprocessor {
  fs::path open_file_;
  fs::path internal_dir_;

  // Reference member for polymorphism
  std::istream& stream_;

  // Use buffer to simplify lookahead logic with preprocessing
  std::deque<PosChar> peek_buffer_;

  inline void appendToBuffer(const PosStr& chars) {
    peek_buffer_.insert(peek_buffer_.end(), chars.begin(), chars.end());
  }

  // Position of current character required to report where there are errors
  PosChar current_char_;

  // Preprocessor directives must appear at a line start (ignoring whitespace)
  bool line_start_ = true;

  // Double quoted string literals are not preprocessed
  bool in_doubles_ = false;

  // May be used recursively to expand nested macros and arguments
  bool expand_only_ = false;

  // The current conditional branch directive being prebuffered
  std::string branch_directive_ = "";
  // Whether the if clause is true or false (used to follow else)
  bool branch_condition_ = false;

  void error(int, int, std::string);
  void advance();
  void skipComment();

  // Regular map since macros cannot be overloaded
  std::unordered_map<std::string, MacroDefinition> macros_;

  // When used in subcontext parameter replacement may take place
  std::unordered_map<std::string, MacroArg> params_;

  // Set of macros that aren't expanded in the current context to prevent
  // recursion
  std::unordered_set<std::string> macro_context_;

  // Included files to error on and prevent recursion
  std::unordered_set<std::string> inclusion_context_;

  inline bool isMacro(const std::string& word) {
    return macros_.find(word) != macros_.end();
  }

  inline bool isParam(const std::string& word) {
    return params_.find(word) != params_.end();
  }

  inline bool isRecursive(const std::string& word) {
    return macro_context_.find(word) != macro_context_.end();
  }

  inline bool isRecursiveInclude(const fs::path& path) {
    return inclusion_context_.find(path) != inclusion_context_.end();
  }

  // Class recursively used to expand nested macro usage contexts
  Preprocessor(
      std::istream&,
      std::unordered_map<std::string,
                         MacroArg>&,  // Parameter map for replacement (only
                                      // relevant to macro body expansion)
      std::unordered_map<std::string,
                         MacroDefinition>&,  // Set of defined macros for
                                             // expansion (nested)
      std::unordered_set<std::string>&       // Set of macros not to expand
                                             // (prevents macro recursion)
  );

  Preprocessor(std::istream&, fs::path, fs::path,
               std::unordered_set<std::string>&  // Set of paths not to include
                                                 // (prevent recursion)
  );

  std::string getWord();
  std::vector<MacroArg> getArgs(const std::string&);

  void handleDirective();
  void includeFile(const PosStr&);
  void defineMacro(const PosStr&);
  void undefineMacro(const PosStr&);
  void branchDirective(const std::string&, const PosStr&);
  void processWord();

  PosChar nextChar();

 public:
  Preprocessor(std::istream&, fs::path = "fake.sqf", fs::path = "internal");
  PosChar get();
  PosChar peek(size_t = 1);
  PosStr getAll();
};
