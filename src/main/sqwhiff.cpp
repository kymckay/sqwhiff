#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../analyzer/analyzer.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../preprocessor/preprocessor.h"
#include "../rules/all_rules.h"
#include "./argparser.h"

namespace fs = std::filesystem;

// TODO: Add way to input a directory to fully analyze
static void show_usage() {
  std::cerr << "Usage: sqwhiff <option(s)> FILE_PATHS\n"
            << "Options:\n"
            << "\t--help\t\tShow this help message and exit\n"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    show_usage();
    return 1;
  }

  ArgParser args(argc, argv);

  if (args.hasArgument("--help")) {
    show_usage();
    return 0;
  }

  int errorc = 0;
  for (auto &&file : args.getRemaining()) {
    fs::path file_path(file);
    std::ifstream file_in(file_path);

    if (file_in.is_open()) {
      Preprocessor preproc(file_in, file_path);
      Lexer lex(preproc);
      Parser p(lex);
      Analyzer a(p);
      errorc += a.analyze(std::cout, all_rules);
      file_in.close();
    } else {
      std::cerr << "Unable to open file: " << file << "\n";
      errorc++;  // Bad usage should count as an error
    }
  }

  std::cout << "\nsqwhiff result: " << errorc << " error(s) encountered"
            << std::endl;

  return errorc;
}
