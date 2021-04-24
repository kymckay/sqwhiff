#include "parser.h"
#include "lexer.h"
#include "token.h"

// Consume a file's tokens and structure them together (parse the file)
void Parser::parse(Lexer &scanner) {

    Token thisToken = scanner.nextToken();
    while (!thisToken.content.empty())
    {
        // Strings and comment contexts consume other tokens until they end
        if (lineComment_ || blockComment_ || !inString_.content.empty())
        {
            if (lineComment_ && thisToken.content == "\n")
                lineComment_ = false;
            else if (blockComment_ && thisToken.content == "*/")
                blockComment_ = false;
            else if (!inString_.content.empty())
            {
                inString_.content.append(thisToken.content);
                if (thisToken.content.back() == inString_.content.front())
                    // TODO string completed
            }


            continue;
        }

        // Strings and comment beginnings
        if (thisToken.content == "//")
            lineComment_ = true;
        else if (thisToken.content == "/*")
            blockComment_ = true;
        else if (thisToken.content == "'" || thisToken.content == "\"")
            inString_ = thisToken;
    }
}
