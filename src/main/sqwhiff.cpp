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
  std::cerr
      << "Usage: sqwhiff <option(s)> FILE_PATHS\n"
      << "Options:\n"
      << "\t--help\t\tShow this help message and exit\n"
      << "\t--internal DIRECTORY_PATH\t\tSpecify directory to be\n"
         "\t\t\tused as the RV engine internal filesystem root. This is the\n"
         "\t\t\tpath used for preprocessor inclusion of file paths that begin\n"
         "\t\t\twith the \\ character\n"
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

  std::string internal_root = "";
  if (args.hasArgument("--internal")) {
    internal_root = args.getArgument("--internal");

    if (!fs::is_directory(internal_root)) {
      std::cerr << "Provided internal filesystem root is not a directory: "
                << internal_root << std::endl;
      return 1;
    }
  }

  int errorc = 0;
  for (auto &&file : args.getRemaining()) {
    fs::path file_path(file);
    std::ifstream file_in(file_path);

    if (file_in.is_open()) {
      Preprocessor preproc(file_in, file_path, internal_root);
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
