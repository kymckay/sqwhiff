#include "./analyzer.h"

#include <ostream>
#include <vector>

#include "./semantic_error.h"

Analyzer::Analyzer(Parser &p) : parser_(p){};

int Analyzer::analyze(std::ostream &out, rule_set &rules) {
  ast_ptr tree = nullptr;
  try {
    tree = parser_.parse();
  } catch (const PreprocessingError &e) {
    out << e.what();
    return 1;
  } catch (const LexicalError &e) {
    out << e.what();
    return 1;
  } catch (const SyntaxError &e) {
    out << e.what();
    return 1;
  }

  int errorc = 0;

  for (auto &&s : rules) {
    std::vector<SemanticError> errors = s.second->getErrors(*tree);
    for (auto &&e : errors) {
      out << e.what() << '\n';
    }

    errorc += errors.size();
  }

  return errorc;
};
