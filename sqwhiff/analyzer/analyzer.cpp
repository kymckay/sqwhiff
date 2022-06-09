#include "sqwhiff/analyzer/analyzer.hpp"

#include "sqwhiff/errors/preprocessing_error.hpp"

Analyzer::Analyzer(Parser &p) : parser_(p){};

error_storage Analyzer::analyze(rule_set &rules) {
  auto result = error_storage();

  ast_ptr tree = nullptr;
  try {
    tree = parser_.parse();
  } catch (const sqwhiff::PreprocessingError &e) {
    result[e.uid()] = std::make_unique<sqwhiff::PreprocessingError>(e);
    return result;
  } catch (const sqwhiff::LexicalError &e) {
    result[e.uid()] = std::make_unique<sqwhiff::LexicalError>(e);
    return result;
  } catch (const sqwhiff::SyntaxError &e) {
    result[e.uid()] = std::make_unique<sqwhiff::SyntaxError>(e);
    return result;
  }

  for (auto &&s : rules) {
    result.merge(s.second->getErrors(*tree));
  }

  return result;
};
