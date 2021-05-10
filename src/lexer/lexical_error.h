#include <string>
#include <stdexcept>

class LexicalError : public std::runtime_error
{
public:
    LexicalError(int line, int col, std::string msg) : std::runtime_error(std::to_string(line) + ":" + std::to_string(col) + " LexerError: " + msg) {}
};