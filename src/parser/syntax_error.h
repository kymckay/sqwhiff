#include <string>
#include <stdexcept>

class SyntaxError : public std::runtime_error
{
public:
    SyntaxError(int line, int col, std::string msg) : std::runtime_error(std::to_string(line) + ":" + std::to_string(col) + " SyntaxError: " + msg) {}
};