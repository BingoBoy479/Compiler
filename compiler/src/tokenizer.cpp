#include "../include/tokenizer.h"

#include <iostream>

#include <unordered_set>

using namespace std;

vector<Token> file_tokens;

unordered_set<string_view> keywords =
{
    "start",
    "int",
    "for",
    "while",
    "if",
    "else",
    "function",
    "return"
};

inline bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

inline bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

inline bool is_alnum(char c)
{
    return is_alpha(c) || is_digit(c);
}
inline bool endsExpression(TokenType t)
{
    return
        t == Word ||
        t == Number ||
        t == RightParen;
}
inline bool is_space(char c)
{
    return c == ' '  ||
           c == '\t' ||
           c == '\n' ||
           c == '\r' ||
           c == '\f' ||
           c == '\v';
}

vector<Token> tokenize(const string& source)
{
    vector<Token> file_tokens;
    file_tokens.clear();
    file_tokens.reserve(source.size() / 4);

    const char* begin = source.data();
    const char* p = begin;
    const char* end = begin + source.size();

    while (p < end)
    {
        if (is_space(*p))
        {
            ++p;
            continue;
        }

        uint32_t start =
            static_cast<uint32_t>(p - begin);

        // Numbers
        if (is_digit(*p))
        {
            ++p;

            while (p < end && is_digit(*p))
                ++p;

            file_tokens.push_back({
                Number,
                start,
                static_cast<uint32_t>(
                    p - begin - start
                )
            });

            continue;
        }

        // Identifiers / Keywords
        if (is_alpha(*p) || *p == '_')
        {
            ++p;

            while (
                p < end &&
                (is_alnum(*p) || *p == '_')
            )
            {
                ++p;
            }

            uint32_t len =
                static_cast<uint32_t>(
                    p - begin - start
                );

            string_view word(
                begin + start,
                len
            );

            if (keywords.count(word))
            {
                if (word == "start")
                {
                    file_tokens.push_back({
                        KwStart,
                        start,
                        len
                    });
                }
                else if( word == "function")
                {
                    file_tokens.push_back({
                        KwFunction,
                        start,
                        len
                    });
                }
                else if( word == "if")
                {
                    file_tokens.push_back({
                        KwIf,
                        start,
                        len
                    });
                }
                else if( word == "else")
                {
                    file_tokens.push_back({
                        KwElse,
                        start,
                        len
                    });
                }
                else if( word == "while")
                {
                    file_tokens.push_back({
                        KwWhile,
                        start,
                        len
                    });
                }
                else if( word == "for")
                {
                    file_tokens.push_back({
                        KwFor,
                        start,
                        len
                    });
                }
                else if( word == "return")
                {
                    file_tokens.push_back({
                        KwReturn,
                        start,
                        len
                    });
                }
                else if( word == "int")
                {
                    file_tokens.push_back({
                        KwInt,
                        start,
                        len
                    });
                }
            }
            else
            {
                file_tokens.push_back({
                    Word,
                    start,
                    len
                });
            }

            continue;
        }

        // Symbols
        switch (*p)
        {
            case '+':
                if (
                    p + 1 < end &&
                    (*(p + 1) == '=')
                )
                {
                    file_tokens.push_back({
                        PlusAssign,
                        start,
                        2
                    });

                    p += 2;
                }
                else if (p + 1 < end &&
                    (*(p + 1) == '+'))
                {
                    if
                    (
                        !file_tokens.empty() &&
                        endsExpression(file_tokens.back().name)
                    )
                    {
                        throw runtime_error(
                            "Postfix ++ not supported. Use Prefix"
                        );
                    }
                    
                    file_tokens.push_back({
                        Increase,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Plus,
                        start,
                        1
                    });

                    ++p;
                }
                continue;

            case '-':
                if (
                    p + 1 < end &&
                    (*(p + 1) == '=')
                )
                {
                    file_tokens.push_back({
                        MinusAssign,
                        start,
                        2
                    });

                    p += 2;
                }
                else if(p + 1 < end &&
                    (*(p + 1) == '-'))
                {
                    
                    if
                    (
                        !file_tokens.empty() &&
                        endsExpression(file_tokens.back().name)
                    )
                    {
                        throw runtime_error(
                            "Postfix -- not supported. Use Prefix"
                        );
                    }
                    
                    
                    file_tokens.push_back({
                        Decrease,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Minus,
                        start,
                        1
                    });

                    ++p;
                }
                continue;
            case '<':
            {
                if (p + 1 < end && *(p + 1) == '=')
                {
                    file_tokens.push_back({
                        LessEqual,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Less,
                        start,
                        1
                    });

                    ++p;
                }

                continue;
            }

            case '>':
            {
                if (p + 1 < end && *(p + 1) == '=')
                {
                    file_tokens.push_back({
                        GreaterEqual,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Greater,
                        start,
                        1
                    });

                    ++p;
                }

                continue;
            }

            case '=':
            {
                if (p + 1 < end && *(p + 1) == '=')
                {
                    file_tokens.push_back({
                        EqualEqual,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Assign,
                        start,
                        1
                    });

                    ++p;
                }

                continue;
            }
            case '&':
            {
                if(p+1 <end && *(p+1) == '&')
                {
                    file_tokens.push_back (
                        {AndAnd,
                        start,
                        2}
                    );
                    p+=2;
                }
                else throw runtime_error("Expected &: bitAnd not available");
                continue;
            }
            case '|':
            {
                if(p+1 <end && *(p+1) == '|')
                {
                    file_tokens.push_back (
                        {OrOr,
                        start,
                        2}
                    );
                    p+=2;
                }
                else throw runtime_error("Expected |: Pipe not available");
                continue;
            }
            case '!':
            {
                if (p + 1 < end && *(p + 1) == '=')
                {
                    file_tokens.push_back({
                        NotEqual,
                        start,
                        2
                    });

                    p += 2;
                }
                else
                {
                    file_tokens.push_back({
                        Not,
                        start,
                        1
                    });

                    ++p;
                }

                continue;
            }
            
            
            case '(':
                file_tokens.push_back({ LeftParen, start, 1 });
                ++p;
                continue;

            case ')':
                file_tokens.push_back({ RightParen, start, 1 });
                ++p;
                continue;

            case '{':
                file_tokens.push_back({ LeftBrace, start, 1 });
                ++p;
                continue;

            case '}':
                file_tokens.push_back({ RightBrace, start, 1 });
                ++p;
                continue;

            case ';':
                file_tokens.push_back({ Semicolon, start, 1 });
                ++p;
                continue;

            case ',':
                file_tokens.push_back({ Comma, start, 1 });
                ++p;
                continue;

            case '*':
                file_tokens.push_back({ Star, start, 1 });
                ++p;
                continue;

            case '/':
                file_tokens.push_back({ Slash, start, 1 });
                ++p;
                continue;
            
            case '%':
                file_tokens.push_back({ Mod, start, 1 });
                ++p;
                continue;
            default:
                ++p;
                continue;
        }
    }

    file_tokens.push_back({
        End,
        static_cast<uint32_t>(source.size()),
        0
    });
    return file_tokens;
}

void printTokens(std::vector<Token>& file_tokens , const string& source)
{
    for ( auto& token : file_tokens)
    {
        if (token.name == End)
        {
            cout << "END\n";
            break;
        }

        string_view value(
            source.data() + token.start,
            token.length
        );

        const char* type;

        switch(token.name)
        {
            case KwStart:      type = "KwStart"; break;
            case KwFunction:   type = "KwFunction"; break;
            case KwIf:         type = "KwIf"; break;
            case KwElse:       type = "KwElse"; break;
            case KwWhile:      type = "KwWhile"; break;
            case KwFor:        type = "KwFor"; break;
            case KwReturn:     type = "KwReturn"; break;
            case KwInt:        type = "KwInt"; break;

            case Word:         type = "Word"; break;
            case Number:       type = "Number"; break;

            case Plus:         type = "Plus"; break;
            case Increase:     type = "Increase"; break;
            case Minus:        type = "Minus"; break;
            case Decrease:     type = "Decrease"; break;
            case Star:
                type = "Star";
                break;

            case Slash:
                type = "Slash";
                break;

            case Assign:       type = "Assign"; break;
            case PlusAssign:   type = "PlusAssign"; break;
            case MinusAssign:  type = "MinusAssign"; break;

            case EqualEqual:   type = "EqualEqual"; break;

            case Less:         type = "Less"; break;
            case LessEqual:    type = "LessEqual"; break;

            case Greater:      type = "Greater"; break;
            case GreaterEqual: type = "GreaterEqual"; break;

            case AndAnd:       type = "AndAnd"; break;
            case OrOr:         type = "OrOr"; break;
            case Not:          type = "Not"; break;
            case NotEqual:     type = "NotEqual"; break;

            case LeftParen:    type = "LeftParen"; break;
            case RightParen:   type = "RightParen"; break;

            case LeftBrace:    type = "LeftBrace"; break;
            case RightBrace:   type = "RightBrace"; break;

            case Comma:        type = "Comma"; break;
            case Semicolon:    type = "Semicolon"; break;

            default:           type = "Unknown";
        }

        cout
            << type
            << " | "
            << value
            << '\n';
    }
}