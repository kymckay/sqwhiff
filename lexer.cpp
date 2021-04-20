#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Lexer
{
    std::ifstream file_;
    std::vector<std::string> tokens_; // May need to store tokens in a buffer

    const static std::vector<std::string> DELIMITERS;
    const static int DELIMITER_MAX_LENGTH = 2;

public:
    Lexer(std::string);
    ~Lexer();
    std::string nextToken();
};

// All possible SQF token delimiters
const std::vector<std::string> Lexer::DELIMITERS = {"\\\n", "\r\n", ">>", "/*", "*/", "//", "||", "!=", "<=", ">=", "==", "\"", "'", " ", "=", ":", "{", "}", "(", ")", "[", "]", ";", ",", "!", "\n", "\t", "/", "*", "+", "-", "%", "^", ">", "<"};

// Open the file resource immediately for reading
// A lexer should never be instantiated unless the intention is to read the file so this is expected
Lexer::Lexer(std::string file)
{
    file_ = std::ifstream(file);
}

// Close the file resource upon destruction
Lexer::~Lexer()
{
    if (file_.is_open())
        file_.close();
}

// Get the next token, reads through the file until a delimiter is found or EOF is hit
std::string Lexer::nextToken()
{

    std::string token;

    // Prioritise previously extracted delimiter tokens
    if (tokens_.size() > 0)
    {
        token = tokens_.back();
        tokens_.pop_back();
        return token;
    }

    if (file_.is_open())
    {
        // Read next character into this variable
        char curChar;

        // Characters read and appended here
        std::string token;

        while (file_.get(curChar))
        {
            token.push_back(curChar);

            // TODO: if first char is a delimiter and second isn't a token is skipped
            if (token.size() < DELIMITER_MAX_LENGTH)
            {
                continue;
            }

            // Check the end of the string for delimiters in decreasing length
            for (int i = DELIMITER_MAX_LENGTH; i > 0; i--)
            {
                int j = token.length() - i;

                // When a delimiter is found the string can be split and a token is extracted
                if (std::find(DELIMITERS.begin(), DELIMITERS.end(), token.substr(j)) != DELIMITERS.end())
                {
                    // Store the delimiter token for the next call
                    tokens_.push_back(token.substr(j));
                    return token.substr(0, j);
                }
            }
        }

        // EOF was reached, return final token and close
        return token;
        file_.close();
    }

    // Empty token if there's nothing to read
    return ""; // TODO: Throw an exception
}

int main()
{
    Lexer test("test.txt");
    std::string t;
    do
    {
        t = test.nextToken();
        std::cout << t << "\n";
    } while (t != "");
}