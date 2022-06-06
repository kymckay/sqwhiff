#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

// Custom helper class to process command line arguments
class ArgParser {
 private:
  // For simplicity it's assumed an argument can only take one value
  std::unordered_map<std::string, std::string> arg_map_;
  std::vector<std::string> loose_args_;

 public:
  ArgParser(int argc, char** argv);
  bool hasArgument(const std::string&);
  std::string getArgument(const std::string&);
  std::vector<std::string> getRemaining();
};
