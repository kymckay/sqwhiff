#pragma once
#include <deque>
#include <filesystem>
#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "sqwhiff/preprocessor/macro_manager.hpp"
#include "sqwhiff/preprocessor/source_consumer.hpp"
#include "sqwhiff/structures/source_char.hpp"

namespace fs = std::filesystem;

using sqwhiff::MacroManager;
using sqwhiff::SourceChar;
using sqwhiff::SourceConsumer;
using sqwhiff::SourceString;

class Preprocessor {
  fs::path open_file_;
  fs::path internal_dir_ = "";

  SourceConsumer source_;

  // Use buffer to simplify lookahead logic with preprocessing
  std::deque<SourceChar> peek_buffer_;

  inline void appendToBuffer(const SourceString& ss) {
    peek_buffer_.insert(peek_buffer_.end(), ss.chars.begin(), ss.chars.end());
  }

  // Preprocessor directives must appear at a line start (ignoring whitespace)
  bool line_start_ = true;

  // Double quoted string literals are not preprocessed
  bool in_doubles_ = false;

  // The current conditional branch directive being prebuffered
  std::string branch_directive_ = "";
  // Whether the if clause is true or false (used to follow else)
  bool branch_condition_ = false;

  void skipComment();

  // Macro definitions which are shared across inclusion boundaries
  std::shared_ptr<MacroManager<SourceConsumer>> macro_context_;

  // Included files to error on and prevent recursion
  std::unordered_set<std::string> inclusion_context_;

  inline bool isRecursiveInclude(const fs::path& path) {
    return inclusion_context_.find(path.string()) != inclusion_context_.end();
  }

  void handleDirective();
  void includeFile(const SourceString&);
  void undefineMacro(const SourceString&);
  void branchDirective(const std::string&, const SourceString&);

  SourceChar nextChar();

 public:
  // Constructor enables recursive reuse of the class. Used for:
  //   - Nested file inclusion (with shared macro modification)
  Preprocessor(std::istream&, fs::path = "", fs::path = "",
               std::shared_ptr<MacroManager<SourceConsumer>> = nullptr,
               // Set of paths not to include (prevent recursion)
               const std::unordered_set<std::string>* = nullptr);
  SourceChar get();
  SourceChar peek(size_t = 1);
  SourceString getAll();
};
