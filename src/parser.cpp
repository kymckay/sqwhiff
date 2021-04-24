#include "parser.h"
#include "lexer.h"
#include "token.h"

// Consume a file's tokens and structure them together (parse the file)
void Parser::parse(Lexer &scanner) {

    Token thisToken = scanner.nextToken();
    while (!thisToken.raw.empty())
    {
        // Strings and comment contexts consume other tokens until they end
        if (lineComment_ || blockComment_ || !inString_.raw.empty())
        {
            if (lineComment_ && thisToken.raw == "\n")
                lineComment_ = false;
            else if (blockComment_ && thisToken.raw == "*/")
                blockComment_ = false;
            else if (!inString_.raw.empty())
            {
                inString_.raw.append(thisToken.raw);
                if (thisToken.raw.back() == inString_.raw.front())
                    // TODO string completed
            }


            continue;
        }

        // Strings and comment beginnings
        if (thisToken.raw == "//")
            lineComment_ = true;
        else if (thisToken.raw == "/*")
            blockComment_ = true;
        else if (thisToken.raw == "'" || thisToken.raw == "\"")
            inString_ = thisToken;
    }
}
