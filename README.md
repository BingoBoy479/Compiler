# Curated Programming Language Compiler

A compiler for a custom imperative programming language written in C++.

## Features Implemented

- Lexical analysis (tokenizer)
- Abstract Syntax Tree (AST)
- Operator precedence parsing
- Unary operators
- Function calls
- Nested function calls
- Assignment expressions
- Logical and comparison operators

## Currently Working On

- Statement parser
- Declaration parser
- Semantic analysis
- Intermediate representation
- Code generation

## Build

g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o compiler
or
./build.sh

## Example

add(max(1,2), add(3,4))
