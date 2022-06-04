#include "./preprocessor.h"

#include <cctype>
#include <fstream>
#include <sstream>

Preprocessor::Preprocessor(
    std::istream& to_read, fs::path open_file, fs::path internal_dir,
    const std::unordered_set<std::string>* include_context,
    const std::unordered_map<std::string, MacroArg>* macro_params,
    shared_macro_storage macros_defined,
    const std::unordered_set<std::string>* macro_context)
    : stream_(to_read) {
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

  // Macro definitions can carry through and are modified by inclusions
  macros_ = macros_defined;
  if (macros_ == nullptr) {
    macros_ = std::make_shared<macro_storage>();
  }

  // TODO: Unsafely assuming if params given the rest is, should group macro
  // context into a struct or similar
  if (macro_params != nullptr) {
    // When expanding a macro context, don't perform regular preprocessing
    expand_only_ = true;

    params_ = *macro_params;
    macro_context_ = *macro_context;
  }

  // Immediately read in the first character (not an advance, don't want to
  // change state)
  stream_.get(current_char_.c);
}

void Preprocessor::error(int line, int col, std::string msg) {
  throw PreprocessingError(line, col, msg);
}

void Preprocessor::advance() {
  // Increment the line whenever a newline is passed
  if (current_char_ == '\n') {
    current_char_.line++;
    current_char_.column = 0;
    line_start_ = true;
  } else if (line_start_ && !std::isspace(current_char_)) {
    line_start_ = false;
  }

  stream_.get(current_char_.c);

  // When end of stream is reached return EOF character
  if (stream_.eof()) {
    current_char_.c = '\0';
  } else {
    current_char_.column++;
  }
}

void Preprocessor::skipComment() {
  if (stream_.peek() == '/') {
    // Intentionally don't skip the newline at the end (acts as a delimiter)
    while (current_char_ != '\0' && current_char_ != '\n') {
      advance();
    }
  } else if (stream_.peek() == '*') {
    while (current_char_ != '\0' &&
           !(current_char_ == '*' && stream_.peek() == '/')) {
      advance();
    }

    // Skip past the block end: */
    advance();
    advance();
  }
}

std::string Preprocessor::getWord() {
  std::string word;
  while (std::isalnum(current_char_) || current_char_ == '_') {
    word.push_back(current_char_);
    advance();
  }

  return word;
}

std::vector<MacroArg> Preprocessor::getArgs(const std::string& word) {
  PosChar open = current_char_;

  std::vector<MacroArg> args;
  if (current_char_ == '(') {
    advance();

    // Position of each argument applied to replacements in full expansion
    MacroArg arg;
    arg.line = current_char_.line;
    arg.column = current_char_.column;

    int open_p = 1;
    while (current_char_ != '\0' && open_p != 0) {
      // Braces can be used inside arguments as long as the pairing is
      // balanced
      if (current_char_ == '(') {
        open_p++;
      } else if (current_char_ == ')') {
        open_p--;
      }

      // Commas don't split arguments within nested braces
      if (current_char_ == ',' && open_p == 1) {
        args.push_back(arg);
        advance();
        arg.raw.clear();
        arg.line = current_char_.line;
        arg.column = current_char_.column;
      } else if (open_p != 0) {
        arg.raw.push_back(current_char_);
        advance();
      }
    }

    if (open_p != 0) {
      error(open.line, open.column, "Unclosed macro arguments '" + word + "('");
    } else {
      // Final argument ends on closing brace
      args.push_back(arg);
      advance();
    }
  }

  return args;
}

void Preprocessor::handleDirective() {
  // Directive position important for errors and macros
  PosChar hash = current_char_;

  // Skip the #
  advance();

  std::string instruction;
  while (std::isalpha(current_char_) || current_char_ == '\\') {
    // Even the instruction itself can be extended across lines
    if (current_char_ == '\\') {
      if (stream_.peek() == '\n') {
        advance();
        advance();
      } else {
        break;
      }
    } else {
      instruction.push_back(current_char_);
      advance();
    }
  }

  // Instruction must immediately following the # (no spaces allowed)
  if (instruction.empty()) {
    error(hash.line, hash.column,
          "Invalid preprocessor directive, no instruction immediately "
          "following the # character");
  }

  // Space characters (not whitespace) between the instruction and body are
  // skipped
  while (current_char_ == ' ') {
    advance();
  }

  // Remaining logical line is the body of the directive
  PosStr body;
  while (current_char_ != '\n' && current_char_ != '\0') {
    // Logical line can be extended by escaped newlines (anywhere in the
    // directive)
    if (current_char_ == '\\' && stream_.peek() == '\n') {
      advance();
      advance();
    } else {
      body.push_back(current_char_);
      advance();
    }
  }

  if (instruction == "include") {
    includeFile(body);
  } else if (instruction == "define") {
    defineMacro(body);
  } else if (instruction == "undef") {
    undefineMacro(body);
  } else if (instruction == "if") {
    if (branch_directive_ == instruction) {
      error(hash.line, hash.column,
            "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "ifdef") {
    if (branch_directive_ == instruction) {
      error(hash.line, hash.column,
            "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "ifndef") {
    if (branch_directive_ == instruction) {
      error(hash.line, hash.column,
            "Cannot nest #" + instruction + " directives");
    }
    branchDirective(instruction, body);
  } else if (instruction == "else") {
    if (branch_directive_.empty()) {
      error(hash.line, hash.column,
            "Cannot use #else with no preceeding #if, #ifdef or #ifndef "
            "directive");
    }
    branchDirective(instruction, body);
  } else if (instruction == "endif") {
    branch_directive_ = "";
    branch_condition_ = false;
  } else {
    error(hash.line, hash.column,
          "Unrecognised preprocessor directive '#" + instruction + "'");
  }
}

void Preprocessor::includeFile(const PosStr& toInclude) {
  const PosChar delimiter = toInclude.front();
  const PosChar delimiterEnd = toInclude.back();

  // File name must be wrapped in double quotes or angled brackets
  if (!(delimiter == '"' && delimiterEnd == '"') &&
      !(delimiter == '<' && delimiterEnd == '>')) {
    error(delimiter.line, delimiter.column,
          "Malformed #include directive: " +
              std::string(toInclude.begin(), toInclude.end()));
    return;
  }

  // File path does not include delimiters
  std::string filename(toInclude.begin() + 1, toInclude.end() - 1);

  // Included path could be internal, user must specify a directory for this
  fs::path abs_path;
  if (filename.front() == '\\') {
    if (internal_dir_.empty() || !fs::is_directory(internal_dir_)) {
      error(delimiter.line, delimiter.column,
            "Invalid internal directory given to find included file: " +
                filename);
    }

    filename.erase(filename.begin());
    abs_path = fs::absolute(internal_dir_ / filename);
  } else {
    // Included path could be relative, set CWD to resolve correctly
    fs::current_path(open_file_.parent_path());
    abs_path = fs::absolute(filename);
  }

  // Recursive inclusion of a file results in RV engine crashing
  if (isRecursiveInclude(abs_path)) {
    error(delimiter.line, delimiter.column,
          "Recursive inclusion of file: " + filename);
  }

  // Open file as a stream
  std::ifstream file(abs_path);
  if (file.is_open()) {
    Preprocessor pp(file, abs_path, internal_dir_, &inclusion_context_, nullptr,
                    macros_);

    appendToBuffer(pp.getAll());

    file.close();
  } else {
    error(delimiter.line, delimiter.column,
          "Included file not found: " + filename);
  }

  // TODO: Pull up changes defined macros
}

void Preprocessor::defineMacro(const PosStr& definition) {
  // Macro ID must start with alpha or underscore, can contain digits after
  PosChar initial = definition[0];
  if (initial != '_' && !std::isalpha(initial)) {
    error(initial.line, initial.column,
          "Macro ID must start with an alpha character or _, found '" +
              std::string(1, initial) + "'");
  }

  bool inParams = false;
  bool skipSpace = false;
  bool inBody = false;
  std::string keyword;
  std::string param;
  MacroDefinition m;
  for (PosChar c : definition) {
    if (inBody) {
      m.body.push_back(c);
    } else if (skipSpace) {
      // Spaces (not whitespace) following the head are ignored
      if (c != ' ') {
        skipSpace = false;
        inBody = true;
        m.body.push_back(c);
      }
    } else if (inParams) {
      if (c == ',') {
        m.params.push_back(param);
        param.clear();
      } else if (c == ')') {
        // Trailing comma is valid
        if (!param.empty()) {
          m.params.push_back(param);
        }

        inParams = false;
        skipSpace = true;
      }
      // Horizontal whitespace around parameters is ignored
      else if (!std::isspace(c)) {
        if (param.empty() && c != '_' && !std::isalpha(c)) {
          error(c.line, c.column,
                "Macro parameter ID must start with an alpha "
                "character or _, found '" +
                    std::string(1, c) + "'");
        }

        param.push_back(c);
      }
    } else {
      if (std::isalnum(c) || c == '_') {
        keyword.push_back(c);
      } else {
        // Parameters are optional, can be object like
        if (c == '(') {
          inParams = true;
        } else {
          skipSpace = true;
        }
      }
    }
  }

  (*macros_)[keyword] = m;
}

void Preprocessor::undefineMacro(const PosStr& undef) {
  std::string keyword;
  for (char c : undef) {
    if (std::isalnum(c) || c == '_') {
      keyword.push_back(c);
    }
  }

  macros_->erase(keyword);
}

void Preprocessor::branchDirective(const std::string& instruction,
                                   const PosStr& body) {
  branch_directive_ = instruction;

  std::string word = "";
  for (auto&& c : body) {
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
    follow_branch = isMacro(word);
  } else if (instruction == "ifndef") {
    follow_branch = !isMacro(word);
  }

  // Store whether branch followed to handle else directive
  branch_condition_ = follow_branch;

  if (follow_branch) {
    PosStr block = PosStr();

    // TODO: This is a hacky fix for a minor problem, see issue #30
    // Dummy character is immediately removed by recursive call to nextChar
    if (instruction == "else") {
      block.push_back(PosChar());
    }

    while (branch_directive_ == instruction) {
      block.push_back(nextChar());
    }

    appendToBuffer(block);
  } else {
    while (branch_directive_ == instruction) {
      nextChar();
    }
  }
}

// Obtains next word and arguments if present and expands to a macro in the
// buffer
void Preprocessor::processWord() {
  // Position of the macro applied to all expanded characters
  PosChar initial = current_char_;

  // May just be object like
  std::string word = getWord();

  MacroDefinition macro_def;
  std::vector<MacroArg> args;  // May be function like

  // Parameter replacement happens before macro expansion
  if (isParam(word)) {
    appendToBuffer(params_.at(word).chars);
    return;
  } else if (isMacro(word) && !isRecursive(word)) {
    macro_def = macros_->at(word);
    args = getArgs(word);
  } else {
    // Just a normal word, push to buffer
    PosChar pc;
    pc.line = initial.line;
    pc.column = initial.column;
    for (char& c : word) {
      pc.c = c;
      peek_buffer_.push_back(pc);
      pc.column++;
    }

    return;
  }

  // The real preprocessor outputs something in such cases, but for now throw
  // an error
  if (args.size() != macro_def.params.size()) {
    error(initial.line, initial.column,
          "Invalid number of macro arguments for '" + word +
              "' supplied, found " + std::to_string(args.size()) +
              ", expected " + std::to_string(macro_def.params.size()));
  }

  // Arguments are expanded before parameter replacement
  for (MacroArg& arg : args) {
    std::stringstream arg_ss(arg.raw);
    std::unordered_map<std::string, MacroArg> no_args;
    Preprocessor pp(arg_ss, open_file_, internal_dir_, nullptr, &no_args,
                    macros_, &macro_context_);
    arg.chars = pp.getAll();

    // Update reference position for all expanded argument characters
    for (PosChar& pc : arg.chars) {
      pc.line += arg.line - 1;
      pc.column += arg.column - 1;
    }
  }

  // Construct a map of parameter replacements
  std::unordered_map<std::string, MacroArg> param_map;
  for (size_t i = 0; i < args.size(); ++i)
    param_map[macro_def.params[i]] = args[i];

  // Prepare the set of macros to ignore in the body (prevent infinite
  // recursion)
  std::unordered_set<std::string> body_context(macro_context_);
  body_context.insert(word);

  // Finally macro body is expanded and processed (done sequentially to
  // preserve correct behaviour)
  std::stringstream body_ss(macro_def.body);
  Preprocessor pp(body_ss, open_file_, internal_dir_, nullptr, &param_map,
                  macros_, &body_context);
  appendToBuffer(pp.getAll());
}

PosChar Preprocessor::nextChar() {
  // Preprocessing does not occur within double quoted string literals
  if (!in_doubles_) {
    // Comments are irrelevant (block and line)
    if (!expand_only_ && current_char_ == '/' &&
        (stream_.peek() == '/' || stream_.peek() == '*')) {
      skipComment();
    }
    // Preprocessor directives indicated by # at line start
    else if (!expand_only_ && line_start_ && current_char_ == '#') {
      handleDirective();
      return get();
    } else if (macro_context_.size() > 0 && current_char_ == '#') {
      // Concatenate the previous and following tokens (i.e. skip these
      // characters)
      if (stream_.peek() == '#') {
        advance();
        advance();
        return get();
      }
      // Stringize the following word token
      else {
        // Position will become macro position later
        PosChar quote;
        quote.c = '"';
        advance();

        processWord();
        peek_buffer_.push_back(quote);
        return quote;
      }
    } else if (std::isalpha(current_char_) || current_char_ == '_') {
      processWord();
      return get();
    }
  }

  // Each double quote encountered inverts the context
  if (current_char_ == '"') {
    in_doubles_ = !in_doubles_;
  }

  PosChar c = current_char_;

  // Remember to actually progress through the input
  advance();

  return c;
}

PosChar Preprocessor::get() {
  // Pull from the buffer first if any peek has occured
  if (!peek_buffer_.empty()) {
    PosChar p = peek_buffer_.front();
    peek_buffer_.pop_front();
    return p;
  }

  return nextChar();
}

// Allows looking ahead to future characters in order to differentiate tokens
// that start the same
PosChar Preprocessor::peek(size_t peek_by) {
  while (peek_buffer_.size() < peek_by) {
    peek_buffer_.push_back(nextChar());
  }

  // Convert peek request to 0-indexed
  return peek_buffer_.at(peek_by - 1);
}

// Processes the whole input and returns the resulting sequence of positioned
// characters
PosStr Preprocessor::getAll() {
  PosChar c = get();
  PosStr result;
  while (c != '\0') {
    result.push_back(c);
    c = get();
  }

  return result;
}
