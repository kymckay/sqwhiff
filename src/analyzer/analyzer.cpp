#include "./analyzer.h"
#include "./semantic_error.h"
#include <ostream>
#include <vector>

Analyzer::Analyzer(Parser &p) : parser_(p){};

int Analyzer::analyze(std::ostream &out, rule_set &rules)
{
    ast_ptr tree = nullptr;
    try
    {
        tree = parser_.parse();
    }
    catch (const PreprocessingError &e)
    {
        out << e.what();
        return 1;
    }
    catch (const LexicalError &e)
    {
        out << e.what();
        return 1;
    }
    catch (const SyntaxError &e)
    {
        out << e.what();
        return 1;
    }

    int errorc = 0;

    for (auto &&s : rules)
    {
        std::vector<SemanticError> errors = s->getErrors(*tree);
        for (auto &&e : errors)
        {
            out << e.what();
        }

        errorc += errors.size();
    }

    return errorc;
};
