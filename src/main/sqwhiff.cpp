#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../analyzer/analyzer.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../preprocessor/preprocessor.h"
#include "../rules/all_rules.h"

namespace fs = std::filesystem;

// TODO: Add way to input a directory to fully analyze
static void show_usage() {
  std::cerr << "Usage: sqwhiff <option(s)> FILE_PATHS\n"
            << "Options:\n"
            << "\t-h,--help\t\tShow this help message and exit\n"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    show_usage();
    return 1;
  }

  std::string p1(argv[1]);
  if (p1 == "-h" || p1 == "--help") {
    show_usage();
    return 0;
  }

  int errorc = 0;
  for (int i = 1; i < argc; i++) {
    fs::path file_path(argv[i]);
    std::ifstream file_in(file_path);

    if (file_in.is_open()) {
      Preprocessor preproc(file_in, file_path);
      Lexer lex(preproc);
      Parser p(lex);
      Analyzer a(p);
      errorc += a.analyze(std::cout, all_rules);
      file_in.close();
    } else {
      std::cerr << "Unable to open file: " << argv[i] << "\n";
      errorc++;  // Bad usage should count as an error
    }
  }

  std::cout << "\nsqwhiff result: " << errorc << " error(s) encountered"
            << std::endl;

  return errorc;
}
