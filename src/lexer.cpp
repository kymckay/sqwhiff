#include "lexer.h"
#include "token.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

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
Token Lexer::nextToken()
{
    // Characters may have previously been read to the buffer
    std::string text = text_buffer_;
    text_buffer_.clear();

    if (file_.is_open())
    {
        // Read next character into this variable
        char cur_char;

        while (file_.get(cur_char))
        {
            text.push_back(cur_char);

            // Increment the line whenever a newline is found
            if (cur_char == '\n') line_++;

            // More text can follow, can't check for delimiters until enough characters exist to match the longest
            if (text.length() < MAX_DELIM_LENGTH)
            {
                continue;
            }

            std::string::size_type b_pos = findNextBoundary(text);
            if (b_pos != std::string::npos)
            {
                text_buffer_ = text.substr(b_pos);
                // Newlines may have been processed to buffer already and updated the line count
                return Token(text.substr(0, b_pos), line_ - bufferedCharCount('\n'));
            }
        }

        // EOF was reached
        file_.close();
    }

    // Process any remining buffered characters
    if (!text.empty())
    {
        std::string::size_type b_pos = findNextBoundary(text);
        if (b_pos != std::string::npos)
        {
            text_buffer_ = text.substr(b_pos);
            // Newlines may have been processed to buffer already and updated the line count
            return Token(text.substr(0, b_pos), line_ - bufferedCharCount('\n'));
        }
    }

    // Will be empty string after buffer is depleted and EOF reached
    return Token(text, line_);
}

// Returns the index of the first token boundary encountered
std::string::size_type Lexer::findNextBoundary(const std::string& text)
{
    int len = text.length();

    // When the text is short, check for delimiters from the start
    if (len <= MAX_DELIM_LENGTH)
    {
        for (int i = len; i > 0; i--)
        {
            if (std::find(DELIMITERS.begin(), DELIMITERS.end(), text.substr(0, i)) != DELIMITERS.end())
            {
                return i;
            }
        }
    }
    // Beyond the delimiter length we only need to check the end of the text
    else
    {
        for (int i = MAX_DELIM_LENGTH; i > 0; i--)
        {
            std::string::size_type s_pos = len - i;
            if (std::find(DELIMITERS.begin(), DELIMITERS.end(), text.substr(s_pos)) != DELIMITERS.end())
            {
                return s_pos;
            }
        }
    }

    return std::string::npos;
}

int Lexer::bufferedCharCount(const char c) {
    return std::count(text_buffer_.begin(), text_buffer_.end(), c);
}

int main()
{
    Lexer test("test.txt");
    Token t;
    do
    {
        t = test.nextToken();
        std::cout << t.line << ": " << t.content << "\n";
    } while (t.content != "");
}