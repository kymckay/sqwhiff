#include "./analyzer.h"

#include <ostream>
#include <vector>

Analyzer::Analyzer(Parser &p) : parser_(p){};

inline void log_error(std::ostream &out, const std::string &err) {
  out << '\n' << err << '\n';
};

int Analyzer::analyze(std::ostream &out, rule_set &rules) {
  ast_ptr tree = nullptr;
  try {
    tree = parser_.parse();
  } catch (const PreprocessingError &e) {
    log_error(out, e.pretty());
    return 1;
  } catch (const LexicalError &e) {
    log_error(out, e.pretty());
    return 1;
  } catch (const SyntaxError &e) {
    log_error(out, e.pretty());
    return 1;
  }

  int errorc = 0;

  for (auto &&s : rules) {
    std::vector<SemanticError> errors = s.second->getErrors(*tree);
    for (auto &&e : errors) {
      log_error(out, e.pretty());
    }

    errorc += errors.size();
  }

  return errorc;
};
