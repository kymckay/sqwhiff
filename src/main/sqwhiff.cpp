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

static int analyzeFile(fs::path file_path, std::string internal_root) {
  file_path = fs::absolute(file_path);
  std::ifstream file_in(file_path);

  if (file_in.is_open()) {
    std::cout << "\nAnalyzing " << file_path << '\n';

    Preprocessor preproc(file_in, file_path, internal_root);
    Lexer lex(preproc);
    Parser p(lex);
    Analyzer a(p);

    error_storage result = a.analyze(all_rules);
    file_in.close();

    for (auto &&[key, err] : result) {
      if (err->file != file_path) {
        std::cout << "\tError in included file " << err->file << '\n';
      }

      std::cout << '\t' << std::to_string(err->line) << ':'
                << std::to_string(err->col) << ' ' << err->type() << " - "
                << err->what() << '\n';
    }

    return result.size();
  }

  std::cerr << "Unable to open file: " << file_path << "\n";
  return 1;  // Bad usage should count as an error
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
  int filec = 0;
  for (auto &&token : args.getRemaining()) {
    fs::path target_path(token);

    if (fs::is_directory(target_path)) {
      for (auto &&target : fs::recursive_directory_iterator(target_path)) {
        if (target.is_regular_file() && target.path().extension() == ".sqf") {
          errorc += analyzeFile(target.path(), internal_root);
          filec++;
        }
      }
    } else {
      errorc += analyzeFile(target_path, internal_root);
      filec++;
    }
  }

  std::cout << "\nsqwhiff result: " << filec << " file(s) processed, " << errorc
            << " error(s) encountered" << std::endl;

  return errorc;
}
