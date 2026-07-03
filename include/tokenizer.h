#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum TokenType
{
    End,
    Word,
    Number,

    KwStart,
    KwFunction,
    KwIf,
    KwElse,
    KwWhile,
    KwFor,
    KwReturn,
    
    KwTypeBegin,
    
    KwInt,

    KwTypeEnd,

    Plus,
    Increase,
    Minus,
    Decrease,
    Star,
    Slash,
    Mod,

    Assign,
    PlusAssign,
    MinusAssign,
    
    AndAnd,
    OrOr, 
    EqualEqual,

    Less,
    LessEqual,

    Greater,
    GreaterEqual,

    Not,
    NotEqual,

    LeftParen,
    RightParen,

    LeftBrace,
    RightBrace,
    
    UnaryMinus,
    Comma,
    Semicolon
};

struct Token
{
    TokenType name;
    uint32_t start;
    uint32_t length;
};
std::vector<Token> tokenize(const std::string& source);
void printTokens(std::vector<Token>& file_tokens,const std::string& source);