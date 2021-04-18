#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Lexer
{
    std::ifstream file_;

public:
    Lexer(std::string);
    ~Lexer();
    std::string nextToken();
};

Lexer::Lexer(std::string file)
{
    file_ = std::ifstream(file);
}

Lexer::~Lexer()
{
    if (file_.is_open())
        file_.close();
}

std::string Lexer::nextToken()
{
    if (file_.is_open())
    {
        char curChar;
        std::vector<char> token;
        while (file_.get(curChar))
        {
            if (curChar == ';') {
                return std::string(token.begin(), token.end());
            } else {
                token.push_back(curChar);
            }
        }
    }
    return ""; // TODO: Throw an error or something
}

int main()
{
    Lexer test("test.txt");
    std::cout << test.nextToken() << " break " << test.nextToken();
}