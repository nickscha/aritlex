/* aritlex.h - v0.2 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Arithmetic Lexer (ARITLEX).

This Test class defines cases to verify that we don't break the excepted behaviours in the future upon changes.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#include "../aritlex.h"   /* Arithmetic Lexer */
#include "../deps/test.h" /* Simple Testing framework    */
#include "../deps/perf.h" /* Simple Performance profiler */
#include "stdio.h"        /* printf */

static s8 *aritlex_token_type_name[] = {
    "TOK_NUM_INTEGER",
    "TOK_PLUS",
    "TOK_MINUS",
    "TOK_MUL",
    "TOK_DIV",
    "TOK_MOD",
    "TOK_LPAREN",
    "TOK_RPAREN",
    "TOK_QMARK",
    "TOK_COLON",
    "TOK_EQ",
    "TOK_NEQ",
    "TOK_LT",
    "TOK_LE",
    "TOK_GT",
    "TOK_GE",
    "TOK_VAR",
    "TOK_EOF",
    "TOK_ERROR",
    "TOK_ASSIGN",
    "TOK_PLUS_EQ",
    "TOK_MINUS_EQ",
    "TOK_MUL_EQ",
    "TOK_DIV_EQ",
    "TOK_MOD_EQ",
    "TOK_AND_AND",
    "TOK_OR_OR",
    "TOK_NOT",
    "TOK_AND",
    "TOK_OR",
    "TOK_XOR",
    "TOK_NOT_BIT",
    "TOK_SHL",
    "TOK_SHR",
    "TOK_INC",
    "TOK_DEC",
    "TOK_STRING",
    "TOK_NUM_FLOAT",
    "TOK_SHL_EQ",
    "TOK_SHR_EQ",
    "TOK_AND_EQ",
    "TOK_OR_EQ",
    "TOK_XOR_EQ"};

static void aritlex_test(void)
{
#define TOKENS_CAPACITY 1024
  aritlex_token tokens[TOKENS_CAPACITY];
  u32 tokens_size = 0;

  s8 *code =
      "42 + 7 - 3 * 2 / 5 % 4 "                      /* arithmetic */
      "a = b += c -= d *= e /= f %= g "              /* assignment & compound*/
      "x && y || !z "                                /* logical*/
      "&h | i ^ j ~k "                               /* bitwise*/
      "<< l >> m <<= n >>= o &= p |= q ^= r "        /* shifts & compound bitwise*/
      "++s --t "                                     /* increment/decrement*/
      "(1 + 2) * (3 - 4) / 5 "                       /* parentheses*/
      "varName _underscore Test123 "                 /* variables*/
      "\"simple string\" \"escape \\\"quote\\\"\" "  /* strings*/
      "0xDEAD 0Xbeef 0b1011 0B1100 1234 "            /* integers*/
      "1.23 0.5f .75 1e10 2.5E-3 3.14f 6.28F "       /* floats*/
      "1_000_000 0x4_2 0b1010_1010 1_234.56_78e1_0"; /* digit separators*/

  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  /* Print tokens */
  {
    u32 i;

    printf("[aritlex] tokens: %d\n", tokens_size);

    for (i = 0; i < tokens_size; ++i)
    {
      if (tokens[i].type == TOK_VAR || tokens[i].type == TOK_STRING)
      {
        printf("[aritlex] [%-20s] %s\n", aritlex_token_type_name[tokens[i].type], tokens[i].val.name);
      }
      else if (tokens[i].type == TOK_NUM_FLOAT)
      {
        printf("[aritlex] [%-20s] %-.6f\n", aritlex_token_type_name[tokens[i].type], (double)tokens[i].val.number_floating);
      }
      else
      {
        printf("[aritlex] [%-20s] %d\n", aritlex_token_type_name[tokens[i].type], tokens[i].val.number_integer);
      }
    }
  }
}

#define TOKENS_CAPACITY 1024
static aritlex_token tokens[TOKENS_CAPACITY];
static u32 tokens_size = 0;

static void aritlex_test_integer(void)
{
  s8 *code = "42";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens_size == 2);
  assert(tokens[0].type == TOK_NUM_INTEGER);
  assert(tokens[0].val.number_integer == 42);
  assert(tokens[1].type == TOK_EOF);
}

static void aritlex_test_float(void)
{
  s8 *code = "3.1415";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens_size == 2);
  assert(tokens[0].type == TOK_NUM_FLOAT);
  assert(tokens[1].type == TOK_EOF);
}

static void aritlex_test_float_exponent(void)
{
  s8 *code = "1.23e2";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_NUM_FLOAT);
  assert(tokens[1].type == TOK_EOF);
}

static void aritlex_test_hex_binary(void)
{
  s8 *code = "0xFF 0b1010";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_NUM_INTEGER);
  assert(tokens[0].val.number_integer == 255);
  assert(tokens[1].type == TOK_NUM_INTEGER);
  assert(tokens[1].val.number_integer == 10);
  assert(tokens[2].type == TOK_EOF);
}

static void aritlex_test_string(void)
{
  s8 *code = "\"hello\\nworld\"";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_STRING);
  assert(tokens[1].type == TOK_EOF);
}

static void aritlex_test_variable(void)
{
  s8 *code = "my_var123";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_VAR);
  assert(tokens[1].type == TOK_EOF);
}

static void aritlex_test_arithmetic_ops(void)
{
  s8 *code = "1 + 2 - 3 * 4 / 5 % 6";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[1].type == TOK_PLUS);
  assert(tokens[3].type == TOK_MINUS);
  assert(tokens[5].type == TOK_MUL);
  assert(tokens[7].type == TOK_DIV);
  assert(tokens[9].type == TOK_MOD);
  assert(tokens[tokens_size - 1].type == TOK_EOF);
}

static void aritlex_test_assignment_ops(void)
{
  s8 *code = "a = 1a b += 2a c -= 3a d *= 4a e /= 5a f %= 6a";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  /* Check some representative assignment tokens */
  assert(tokens[1].type == TOK_ASSIGN);
  assert(tokens[5].type == TOK_PLUS_EQ);
  assert(tokens[9].type == TOK_MINUS_EQ);
  assert(tokens[13].type == TOK_MUL_EQ);
  assert(tokens[17].type == TOK_DIV_EQ);
  assert(tokens[21].type == TOK_MOD_EQ);
}

static void aritlex_test_logic_comparison_ops(void)
{
  s8 *code = "a == b != c < d <= e > f >= g && h || i !j";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[1].type == TOK_EQ);
  assert(tokens[3].type == TOK_NEQ);
  assert(tokens[5].type == TOK_LT);
  assert(tokens[7].type == TOK_LE);
  assert(tokens[9].type == TOK_GT);
  assert(tokens[11].type == TOK_GE);
  assert(tokens[13].type == TOK_AND_AND);
  assert(tokens[15].type == TOK_OR_OR);
  assert(tokens[17].type == TOK_NOT);
}

static void aritlex_test_bitwise_ops(void)
{
  s8 *code = "a & b | c ^ d ~e << f >> g &= h |= i ^= j <<= k >>= l";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  /* Simple check: confirm each operator recognized */
  assert(tokens[1].type == TOK_AND);
  assert(tokens[3].type == TOK_OR);
  assert(tokens[5].type == TOK_XOR);
  assert(tokens[7].type == TOK_NOT_BIT);
  assert(tokens[9].type == TOK_SHL);
  assert(tokens[11].type == TOK_SHR);
  assert(tokens[13].type == TOK_AND_EQ);
  assert(tokens[15].type == TOK_OR_EQ);
  assert(tokens[17].type == TOK_XOR_EQ);
  assert(tokens[19].type == TOK_SHL_EQ);
  assert(tokens[21].type == TOK_SHR_EQ);
}

static void aritlex_test_inc_dec(void)
{
  s8 *code = "++x --y";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_INC);
  assert(tokens[2].type == TOK_DEC);
  assert(tokens[4].type == TOK_EOF);
}

static void aritlex_test_symbols(void)
{
  s8 *code = "( ) ? :";
  aritlex_tokenize(code, aritlex_strlen(code), tokens, TOKENS_CAPACITY, &tokens_size);

  assert(tokens[0].type == TOK_LPAREN);
  assert(tokens[1].type == TOK_RPAREN);
  assert(tokens[2].type == TOK_QMARK);
  assert(tokens[3].type == TOK_COLON);
  assert(tokens[4].type == TOK_EOF);
}

int main(void)
{
  aritlex_test();
  aritlex_test_integer();
  aritlex_test_float();
  aritlex_test_float_exponent();
  aritlex_test_hex_binary();
  aritlex_test_string();
  aritlex_test_variable();
  aritlex_test_arithmetic_ops();
  aritlex_test_assignment_ops();
  aritlex_test_logic_comparison_ops();
  aritlex_test_bitwise_ops();
  aritlex_test_inc_dec();
  aritlex_test_symbols();

  return 0;
}

/*
   -----------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
