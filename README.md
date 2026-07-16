# Curated Programming Language Compiler

A compiler front end for a custom imperative programming language written in modern C++.

The project currently implements lexical analysis, recursive-descent parsing, operator-precedence expression parsing, and Abstract Syntax Tree (AST) generation. The goal is to build a complete compiler including semantic analysis, intermediate representation, optimization, and code generation.

---

## Current Status

### Completed

- Lexical Analysis (Tokenizer)
- Recursive-Descent Parser
- Operator-Precedence Expression Parser (Shunting Yard)
- Abstract Syntax Tree (AST)
- Function Declarations
- Variable Declarations
- Block Statements
- Return Statements
- If / Else Statements
- While Loops
- For Loops
- Function Calls
- Nested Function Calls
- Unary Expressions
- Binary Expressions
- Assignment Expressions
- Compound Assignment Operators (`+=`, `-=`)
- Syntax Error Reporting
- AST Pretty Printer

### In Progress

- Symbol Table
- Scope Management
- Name Resolution
- Semantic Analysis
- Type Checking

### Planned

- Intermediate Representation (IR)
- Optimizations
- Assembly / Machine Code Generation
- Standard Library
- Additional Primitive Types

---

# Language Features

## Primitive Types

```c
int
```

## Variable Declaration

```c
int x = 10;
int y;
```

## Arithmetic Expressions

```c
x = a + b * c;
x = (a + b) * c;
```

## Unary Operators

```c
-x;
!flag;
++x;
--x;
```

## Assignment Operators

```c
x = y;
x += 5;
x -= 2;
```

## Comparison Operators

```c
<
<=
>
>=
==
!=
```

## Logical Operators

```c
&&
||
!
```

## Functions

```c
function int add(int a, int b)
{
    return a + b;
}
```

## Function Calls

```c
add(1, 2);

max(add(1,2), add(3,4));
```

## If / Else

```c
if(x > y)
{
    return x;
}
else
{
    return y;
}
```

## While Loop

```c
while(x > 0)
{
    --x;
}
```

## For Loop

```c
for(int i = 0; i < 10; ++i)
{
    x += i;
}
```

---

# Compiler Pipeline

```
Source File
      │
      ▼
Tokenizer
      │
      ▼
Token Stream
      │
      ▼
Parser
      │
      ▼
Abstract Syntax Tree
      │
      ▼
(Symbol Table)      [In Progress]
      │
      ▼
Semantic Analysis   [Planned]
      │
      ▼
Intermediate Representation
      │
      ▼
Code Generation
```

---

# Project Structure

```
include/
    ast.h
    parser.h
    tokenizer.h

src/
    ast.cpp
    parser.cpp
    tokenizer.cpp
    main.cpp

test/
    *.txt
```

---

# Build

Compile manually

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o compiler
```

or

```bash
./build.sh
```

---

# Running

```bash
./compiler test/example.txt
```

---

# Example Program

```c
function int add(int a, int b)
{
    return a + b;
}

function int max(int a, int b)
{
    if(a > b)
    {
        return a;
    }

    return b;
}

start
{
    int x = -10;
    int y = 20;

    int z = add(x, y);

    if(!(x < y))
    {
        z = max(x, y);
    }

    while(x > 0)
    {
        --x;
    }

    for(int i = 0; i < 10; ++i)
    {
        z += i;
    }

    return z;
}
```

---

# Design

The compiler is implemented using modern C++ and stores all AST nodes using `std::unique_ptr`, providing automatic memory management without manual deletion.

Expression parsing is implemented using a modified Shunting Yard algorithm, while statements and declarations are parsed using recursive-descent parsing.

---

# Roadmap

- [x] Lexer
- [x] Parser
- [x] AST
- [x] Function Parsing
- [x] Statement Parsing
- [x] Control Flow
- [ ] Symbol Table
- [ ] Scope Resolution
- [ ] Semantic Analysis
- [ ] IR Generation
- [ ] Optimizer
- [ ] Assembly Generation