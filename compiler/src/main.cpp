#include "../include/tokenizer.h"
#include "../include/parser.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr
            << "Usage: skk <file>\n";

        return 1;
    }

    ifstream file(
        argv[1],
        ios::binary
    );

    if (!file)
    {
        cerr
            << "Failed to open file\n";

        return 1;
    }

    file.seekg(0, ios::end);

    size_t size =
        static_cast<size_t>(
            file.tellg()
        );

    string source;
    source.resize(size);

    file.seekg(0, ios::beg);

    file.read(
        source.data(),
        size
    );

    auto tokens = tokenize(source);

    printTokens(tokens,source);
    Parser parser(tokens, source);

    while(!parser.isAtEnd())
    {
        auto expr = parser.parseExpression();

        printAST(expr.get());

        parser.consume(
            Semicolon,
            "Expected ';'"
        );
    }
    return 0;
}