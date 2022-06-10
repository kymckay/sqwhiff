#include "sqwhiff/preprocessor/preprocessor.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

#include "sqwhiff/errors/preprocessing_error.hpp"

using sqwhiff::PreprocessingError;

Preprocessor::Preprocessor(
    std::istream& to_read, fs::path open_file, fs::path internal_dir,
    std::shared_ptr<MacroManager> macro_context,
    const std::unordered_set<std::string>* include_context)
    : source_(to_read) {
  // Move to first character straight away
  source_.advance();

  if (include_context != nullptr) {
    inclusion_context_ = *include_context;
  }

  // Ensure paths are absolute since CWD may later change before use
  if (!open_file.empty()) {
    open_file_ = fs::absolute(open_file);
    inclusion_context_.insert(open_file_);
  }

  // Allow internal directory to remain empty, may not be needed
  if (!internal_dir.empty()) {
    internal_dir_ = fs::absolute(internal_dir);
  }

  if (macro_context == nullptr) {
    macro_context_ = std::make_shared<MacroManager>();
  } else {
    macro_context_ = macro_context;
  }
}

void Preprocessor::skipComment() {
  if (source_.peek() == '/') {
    // Intentionally don't skip the newline at the end (acts as a delimiter)
    while (source_.at() != '\0' && source_.at() != '\n') {
      source_.advance();
    }
  } else if (source_.peek() == '*') {
    while (source_.at() != '\0' &&
           !(source_.at() == '*' && source_.peek() == '/')) {
      source_.advance();
    }

    // Skip past the block end: */
    source_.advance();
    source_.advance();
  }
}

void Preprocessor::handleDirective() {
  // Directive position important for errors and macros
  SourceChar hash = source_.at();

  // Skip the #
  source_.advance();

  std::string instruction;
  while (std::isalpha(source_.at()) || source_.at() == '\\') {
    // Even the instruction itself can be extended across lines
    if (source_.at() == '\\') {
      if (source_.peek() == '\n') {
        source_.advance();
        source_.advance();
      } else {
        break;
      }
    } else {
      instruction.push_back(source_.at());
      source_.advance();
    }
  }

  // Instruction must immediately following the # (no spaces allowed)
  if (instruction.empty()) {
    throw PreprocessingError(
        hash,
        "Invalid preprocessor directive, no instruction immediately "
        "following the # character");
  }

  // Space characters (not whitespace) between the instruction and body are
  // skipped
  while (source_.at() == ' ') {
    source_.advance();
  }

  // Remaining logical line is the body of the directive
  SourceString body;
  while (source_.at() != '\n' && source_.at() != '\0') {
    // Logical line can be extended by escaped newlines (anywhere in the
    // directive)
    if (source_.at() == '\\' && source_.peek() == '\n') {
      source_.advance();
      source_.advance();
    } else {
      body.chars.push_back(source_.at());
      source_.advance();
    }
  }

  if (instruction == "include") {
    includeFile(body);
  } else if (instruction == "define") {
    macro_context_->define(body);
  } else if (instruction == "undef") {
    undefineMacro(body);
  } else if (instruction == "if") {
    if (branch_directive_ == instruction) {
      throw PreprocessingError(hash,
                               "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "ifdef") {
    if (branch_directive_ == instruction) {
      throw PreprocessingError(hash,
                               "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "ifndef") {
    if (branch_directive_ == instruction) {
      throw PreprocessingError(hash,
                               "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "else") {
    if (branch_directive_.empty()) {
      throw PreprocessingError(
          hash,
          "Cannot use #else with no preceeding #if, #ifdef or #ifndef "
          "directive");
    }
    branchDirective(instruction, body);
  } else if (instruction == "endif") {
    branch_directive_ = "";
    branch_condition_ = false;
  } else {
    throw PreprocessingError(
        hash, "Unrecognised preprocessor directive '#" + instruction + "'");
  }
}

void Preprocessor::includeFile(const SourceString& toInclude) {
  const SourceChar delimiter = toInclude.chars.front();
  const SourceChar delimiterEnd = toInclude.chars.back();

  // File name must be wrapped in double quotes or angled brackets
  if (!(delimiter == '"' && delimiterEnd == '"') &&
      !(delimiter == '<' && delimiterEnd == '>')) {
    throw PreprocessingError(
        delimiter, "Malformed #include directive: " + (std::string)toInclude);
    return;
  }

  // File path does not include delimiters
  std::string filename(toInclude);
  filename.pop_back();
  filename.erase(0, 1);

  // RV engine supports `\` character for paths, replace with `/` since RV is
  // cross platform
  std::string sanatized(filename.length(), ' ');
  std::replace_copy(filename.begin(), filename.end(), sanatized.begin(), '\\',
                    '/');

  // Included path could be internal, user must specify a directory for this
  fs::path abs_path;
  if (filename.front() == '\\') {
    if (internal_dir_.empty() || !fs::is_directory(internal_dir_)) {
      throw PreprocessingError(
          delimiter,
          "Invalid internal directory given to find included file: " +
              filename);
    }

    sanatized.erase(sanatized.begin());
    abs_path = fs::absolute(internal_dir_ / sanatized);
  } else {
    // Included path could be relative, set CWD to resolve correctly
    fs::current_path(open_file_.parent_path());
    abs_path = fs::absolute(sanatized);
  }

  // Recursive inclusion of a file results in RV engine crashing
  if (isRecursiveInclude(abs_path)) {
    throw PreprocessingError(delimiter,
                             "Recursive inclusion of file: " + filename);
  }

  // Open file as a stream
  std::ifstream file(abs_path);
  if (file.is_open()) {
    Preprocessor pp(file, abs_path, internal_dir_, macro_context_,
                    &inclusion_context_);

    appendToBuffer(pp.getAll());

    file.close();
  } else {
    throw PreprocessingError(delimiter, "Included file not found: " + filename);
  }
}

void Preprocessor::undefineMacro(const SourceString& undef) {
  std::string keyword;
  for (SourceChar c : undef.chars) {
    if (std::isalnum(c) || c == '_') {
      keyword.push_back(c);
    }
  }

  macro_context_->undefine(keyword);
}

void Preprocessor::branchDirective(const std::string& instruction,
                                   const SourceString& body) {
  branch_directive_ = instruction;

  std::string word = "";
  for (SourceChar c : body.chars) {
    word.push_back(c);
  }

  bool follow_branch = false;

  // Skip over else block if first branch was followed
  if (instruction == "else") {
    follow_branch = !branch_condition_;
  } else if (instruction == "if") {
    // TODO: if treated always true for simplicity, requires some interpretation
    follow_branch = true;
  } else if (instruction == "ifdef") {
    follow_branch = macro_context_->isMacro(word);
  } else if (instruction == "ifndef") {
    follow_branch = !(macro_context_->isMacro(word));
  }

  // Store whether branch followed to handle else directive
  branch_condition_ = follow_branch;

  if (follow_branch) {
    SourceString block;

    // TODO: This is a hacky fix for a minor problem, see issue #30
    // Dummy character is immediately removed by recursive call to nextChar
    if (instruction == "else") {
      block.chars.push_back(SourceChar());
    }

    while (branch_directive_ == instruction) {
      block.chars.push_back(nextChar());
    }

    appendToBuffer(block);
  } else {
    while (branch_directive_ == instruction) {
      nextChar();
    }
  }
}

SourceChar Preprocessor::nextChar() {
  // Preprocessing does not occur within double quoted string literals
  if (!in_doubles_) {
    // Comments are irrelevant (block and line)
    if (source_.at() == '/' &&
        (source_.peek() == '/' || source_.peek() == '*')) {
      skipComment();
    }
    // Preprocessor directives indicated by # at line start
    else if (line_start_ && source_.at() == '#') {
      handleDirective();
      return get();
    } else if (std::isalpha(source_.at()) || source_.at() == '_') {
      appendToBuffer(macro_context_->processWord(source_));
      return get();
    }
  }

  // Each double quote encountered inverts the context
  if (source_.at() == '"') {
    in_doubles_ = !in_doubles_;
  }

  SourceChar c = source_.at();

  line_start_ = (c == '\n') || (line_start_ && c == ' ');

  // Remember to actually progress through the input
  SourceChar next = source_.advance();

  return c;
}

SourceChar Preprocessor::get() {
  // Pull from the buffer first if any peek has occured
  if (!peek_buffer_.empty()) {
    SourceChar p = peek_buffer_.front();
    peek_buffer_.pop_front();
    return p;
  }

  return nextChar();
}

// Allows looking ahead to future characters in order to differentiate tokens
// that start the same
SourceChar Preprocessor::peek(size_t peek_by) {
  while (peek_buffer_.size() < peek_by) {
    peek_buffer_.push_back(nextChar());
  }

  // Convert peek request to 0-indexed
  return peek_buffer_.at(peek_by - 1);
}

// Processes the whole input and returns the resulting sequence of positioned
// characters
SourceString Preprocessor::getAll() {
  SourceChar c = get();
  SourceString result;
  while (c != '\0') {
    result.chars.push_back(c);
    c = get();
  }

  return result;
}
