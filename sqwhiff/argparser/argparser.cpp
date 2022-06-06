#include "sqwhiff/argparser/argparser.hpp"

ArgParser::ArgParser(int argc, char** argv) {
  std::string arg_id = "";

  // Ignore program name (first element)
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    // Leading argument identifiers always start with this character
    if (!arg_id.empty()) {
      arg_map_[arg_id] = arg;
      arg_id.clear();
    } else if (arg.front() == '-') {
      arg_id = arg;
      arg_map_[arg_id] = "";
    } else {
      loose_args_.push_back(arg);
    }
  }
}

bool ArgParser::hasArgument(const std::string& arg_id) {
  return arg_map_.find(arg_id) != arg_map_.end();
}

// Not defensive, assumes calling code has checked argument is present first
std::string ArgParser::getArgument(const std::string& arg_id) {
  return arg_map_.at(arg_id);
}

std::vector<std::string> ArgParser::getRemaining() { return loose_args_; }
