# aritlex
A C89 standard compliant, single header, nostdlib (no C Standard Library) Arithmetic Lexer (ARITLEX).

For more information please look at the "aritlex.h" file or take a look at the "examples" or "tests" folder.

> [!WARNING]
> THIS PROJECT IS A WORK IN PROGRESS! ANYTHING CAN CHANGE AT ANY MOMENT WITHOUT ANY NOTICE! USE THIS PROJECT AT YOUR OWN RISK!

<p align="center">
  <a href="https://github.com/nickscha/aritlex/releases">
    <img src="https://img.shields.io/github/v/release/nickscha/aritlex?style=flat-square&color=blue" alt="Latest Release">
  </a>
  <a href="https://github.com/nickscha/aritlex/releases">
    <img src="https://img.shields.io/github/downloads/nickscha/aritlex/total?style=flat-square&color=brightgreen" alt="Downloads">
  </a>
  <a href="https://opensource.org/licenses/MIT">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square" alt="License">
  </a>
  <img src="https://img.shields.io/badge/Standard-C89-orange?style=flat-square" alt="C Standard">
  <img src="https://img.shields.io/badge/nolib-nostdlib-lightgrey?style=flat-square" alt="nostdlib">
</p>

## **Features**
- **C89 compliant** — portable and legacy-friendly  
- **Single-header API** — just include `aritlex.h`  
- **nostdlib** — no dependency on the C Standard Library  
- **Minimal binary size** — optimized for small executables  
- **Cross-platform** — Windows, Linux, MacOs 
- **Strict compilation** — built with aggressive warnings & safety checks  

## Quick Start

Download or clone aritlex.h and include it in your project.

```C
#include "aritlex.h" /* Arithmetic Lexer */

int main() {
    /* Define a buffer to hold the tokens generated */
    #define TOKENS_CAPACITY 1024
    aritlex_token tokens[TOKENS_CAPACITY];
    u32 tokens_size = 0;

    s8 *code = "1 + 3";

    /* Run the lexer */
    aritlex_tokenize(
      code, aritlex_strlen(code), 
      tokens, TOKENS_CAPACITY, &tokens_size
    );

    return 0;
}
```

## Token Overview

| **Category**               | **Token Name**    | **Lexeme / Symbol(s)** | **Description / Example**               |                       |            |
| :------------------------- | :---------------- | :--------------------- | :-------------------------------------- | --------------------- | ---------- |
| **Literals**               | `TOK_NUM_INTEGER` | `42`, `0xFF`, `0b1010` | Integer literals (decimal, hex, binary) |                       |            |
|                            | `TOK_NUM_FLOAT`   | `3.14`, `1e-3`         | Floating-point numbers                  |                       |            |
|                            | `TOK_STRING`      | `"hello\nworld"`       | String literals with escape sequences   |                       |            |
|                            | `TOK_VAR`         | `x`, `y_1`, `_foo`     | Variable or identifier name             |                       |            |
| **Assignment Operators**   | `TOK_ASSIGN`      | `=`                    | Simple assignment                       |                       |            |
|                            | `TOK_PLUS_EQ`     | `+=`                   | Add and assign                          |                       |            |
|                            | `TOK_MINUS_EQ`    | `-=`                   | Subtract and assign                     |                       |            |
|                            | `TOK_MUL_EQ`      | `*=`                   | Multiply and assign                     |                       |            |
|                            | `TOK_DIV_EQ`      | `/=`                   | Divide and assign                       |                       |            |
|                            | `TOK_MOD_EQ`      | `%=`                   | Modulus and assign                      |                       |            |
|                            | `TOK_AND_EQ`      | `&=`                   | Bitwise AND and assign                  |                       |            |
|                            | `TOK_OR_EQ`       | `                      | =`                                      | Bitwise OR and assign |            |
|                            | `TOK_XOR_EQ`      | `^=`                   | Bitwise XOR and assign                  |                       |            |
|                            | `TOK_SHL_EQ`      | `<<=`                  | Left shift and assign                   |                       |            |
|                            | `TOK_SHR_EQ`      | `>>=`                  | Right shift and assign                  |                       |            |
| **Arithmetic Operators**   | `TOK_PLUS`        | `+`                    | Addition                                |                       |            |
|                            | `TOK_MINUS`       | `-`                    | Subtraction                             |                       |            |
|                            | `TOK_MUL`         | `*`                    | Multiplication                          |                       |            |
|                            | `TOK_DIV`         | `/`                    | Division                                |                       |            |
|                            | `TOK_MOD`         | `%`                    | Modulo                                  |                       |            |
| **Increment / Decrement**  | `TOK_INC`         | `++`                   | Increment                               |                       |            |
|                            | `TOK_DEC`         | `--`                   | Decrement                               |                       |            |
| **Comparison Operators**   | `TOK_EQ`          | `==`                   | Equal to                                |                       |            |
|                            | `TOK_NEQ`         | `!=`                   | Not equal to                            |                       |            |
|                            | `TOK_LT`          | `<`                    | Less than                               |                       |            |
|                            | `TOK_LE`          | `<=`                   | Less than or equal to                   |                       |            |
|                            | `TOK_GT`          | `>`                    | Greater than                            |                       |            |
|                            | `TOK_GE`          | `>=`                   | Greater than or equal to                |                       |            |
| **Logical Operators**      | `TOK_AND_AND`     | `&&`                   | Logical AND                             |                       |            |
|                            | `TOK_OR_OR`       | `                      |                                         | `                     | Logical OR |
|                            | `TOK_NOT`         | `!`                    | Logical NOT                             |                       |            |
| **Bitwise Operators**      | `TOK_AND`         | `&`                    | Bitwise AND                             |                       |            |
|                            | `TOK_OR`          | `                      | `                                       | Bitwise OR            |            |
|                            | `TOK_XOR`         | `^`                    | Bitwise XOR                             |                       |            |
|                            | `TOK_NOT_BIT`     | `~`                    | Bitwise NOT                             |                       |            |
|                            | `TOK_SHL`         | `<<`                   | Left shift                              |                       |            |
|                            | `TOK_SHR`         | `>>`                   | Right shift                             |                       |            |
| **Punctuation / Grouping** | `TOK_LPAREN`      | `(`                    | Left parenthesis                        |                       |            |
|                            | `TOK_RPAREN`      | `)`                    | Right parenthesis                       |                       |            |
|                            | `TOK_QMARK`       | `?`                    | Ternary condition start                 |                       |            |
|                            | `TOK_COLON`       | `:`                    | Ternary else separator                  |                       |            |
|                            | `TOK_SEMICOLON`   | `;`                    | Statement terminator                    |                       |            |
| **Special**                | `TOK_ERROR`       | *(invalid input)*      | Lexical error token                     |                       |            |
|                            | `TOK_EOF`         | *(end of input)*       | End of source marker                    |                       |            |


## Run Example: nostdlib, freestsanding

In this repo you will find the "examples/aritlex_win32_nostdlib.c" with the corresponding "build.bat" file which
creates an executable only linked to "kernel32" and is not using the C standard library and executes the program afterwards.

## "nostdlib" Motivation & Purpose

nostdlib is a lightweight, minimalistic approach to C development that removes dependencies on the standard library. The motivation behind this project is to provide developers with greater control over their code by eliminating unnecessary overhead, reducing binary size, and enabling deployment in resource-constrained environments.

Many modern development environments rely heavily on the standard library, which, while convenient, introduces unnecessary bloat, security risks, and unpredictable dependencies. nostdlib aims to give developers fine-grained control over memory management, execution flow, and system calls by working directly with the underlying platform.

### Benefits

#### Minimal overhead
By removing the standard library, nostdlib significantly reduces runtime overhead, allowing for faster execution and smaller binary sizes.

#### Increased security
Standard libraries often include unnecessary functions that increase the attack surface of an application. nostdlib mitigates security risks by removing unused and potentially vulnerable components.

#### Reduced binary size
Without linking to the standard library, binaries are smaller, making them ideal for embedded systems, bootloaders, and operating systems where storage is limited.

#### Enhanced performance
Direct control over system calls and memory management leads to performance gains by eliminating abstraction layers imposed by standard libraries.

#### Better portability
By relying only on fundamental system interfaces, nostdlib allows for easier porting across different platforms without worrying about standard library availability.
