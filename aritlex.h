/* aritlex.h - v0.2 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Arithmetic Lexer (ARITLEX).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef ARITLEX_H
#define ARITLEX_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define ARITLEX_INLINE inline
#elif defined(__GNUC__) || defined(__clang__)
#define ARITLEX_INLINE __inline__
#elif defined(_MSC_VER)
#define ARITLEX_INLINE __inline
#else
#define ARITLEX_INLINE
#endif

#define ARITLEX_API static

typedef char s8;
typedef int s32;

typedef float f32;
typedef double f64;

typedef unsigned char u8;
typedef unsigned int u32;

ARITLEX_API ARITLEX_INLINE u32 aritlex_is_digit(s8 c)
{
  return c >= '0' && c <= '9';
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_is_alpha(s8 c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_is_space(s8 c)
{
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_is_hex(s8 c)
{
  return (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_is_binary(s8 c)
{
  return (c == '0' || c == '1');
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_strcmp(s8 *s1, s8 *s2, u32 size)
{
  s8 *end = s1 + size;

  while (s1 < end)
  {
    if (*s1++ != *s2++)
    {
      return 0;
    }
  }

  return 1;
}

ARITLEX_API ARITLEX_INLINE u32 aritlex_strlen(s8 *str)
{
  u32 len = 0;

  while (*str++)
  {
    ++len;
  }

  return len;
}

ARITLEX_API s32 aritlex_strtol(s8 *str, s8 **endptr, int base)
{
  s32 result = 0;
  int sign = 1;

  if (*str == '-')
  {
    sign = -1;
    str++;
  }
  else if (*str == '+')
  {
    str++;
  }

  if (base == 0)
  {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
      base = 16;
      str += 2;
    }
    else if (str[0] == '0' && (str[1] == 'b' || str[1] == 'B'))
    {
      base = 2;
      str += 2;
    }
    else if (str[0] == '0')
    {
      base = 8;
      str++;
    }
    else
    {
      base = 10;
    }
  }

  while (*str)
  {
    int digit;

    if (*str == '_')
    {
      str++;
      continue;
    } /* skip separator */
    if (*str >= '0' && *str <= '9')
      digit = *str - '0';
    else if (*str >= 'a' && *str <= 'f')
      digit = 10 + (*str - 'a');
    else if (*str >= 'A' && *str <= 'F')
      digit = 10 + (*str - 'A');
    else
      break;

    if (digit >= base)
      break;

    result = result * base + digit;
    str++;
  }

  if (endptr)
    *endptr = (s8 *)str;
  return sign * result;
}

ARITLEX_API f64 aritlex_strtod(s8 *str, s8 **endptr)
{
  f64 result = 0.0;
  f64 sign = 1.0;
  int exponent = 0, exp_sign = 1;
  f64 pow10 = 1.0;
  int i;

  if (*str == '-')
  {
    sign = -1.0;
    str++;
  }
  else if (*str == '+')
  {
    str++;
  }

  /* Integer part */
  while (aritlex_is_digit(*str) || *str == '_')
  {
    if (*str == '_')
    {
      str++;
      continue;
    }
    result = result * 10.0 + (*str - '0');
    str++;
  }

  /* Fraction */
  if (*str == '.')
  {
    f64 base = 0.1;

    str++;
    while (aritlex_is_digit(*str) || *str == '_')
    {
      if (*str == '_')
      {
        str++;
        continue;
      }
      result += (*str - '0') * base;
      base *= 0.1;
      str++;
    }
  }

  /* Exponent */
  if (*str == 'e' || *str == 'E')
  {
    str++;
    if (*str == '-')
    {
      exp_sign = -1;
      str++;
    }
    else if (*str == '+')
    {
      str++;
    }

    while (aritlex_is_digit(*str) || *str == '_')
    {
      if (*str == '_')
      {
        str++;
        continue;
      }
      exponent = exponent * 10 + (*str - '0');
      str++;
    }
  }

  if (endptr)
    *endptr = (s8 *)str;

  /* Apply exponent */
  for (i = 0; i < exponent; i++)
    pow10 *= 10.0;
  if (exp_sign < 0)
    result /= pow10;
  else
    result *= pow10;

  return sign * result;
}

ARITLEX_API f32 aritlex_strtof(s8 *str, s8 **endptr)
{
  return (f32)aritlex_strtod(str, endptr);
}

typedef enum aritlex_token_type
{
  TOK_NUM_INTEGER,
  TOK_PLUS,
  TOK_MINUS,
  TOK_MUL,
  TOK_DIV,
  TOK_MOD,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_QMARK,
  TOK_COLON,
  TOK_EQ,
  TOK_NEQ,
  TOK_LT,
  TOK_LE,
  TOK_GT,
  TOK_GE,
  TOK_VAR,
  TOK_EOF,
  TOK_ERROR,
  TOK_ASSIGN,   /* = */
  TOK_PLUS_EQ,  /* += */
  TOK_MINUS_EQ, /* -= */
  TOK_MUL_EQ,   /* *= */
  TOK_DIV_EQ,   /* /= */
  TOK_MOD_EQ,   /* %= */
  TOK_AND_AND,  /* && */
  TOK_OR_OR,    /* || */
  TOK_NOT,      /* ! */
  TOK_AND,      /* & */
  TOK_OR,       /* | */
  TOK_XOR,      /* ^ */
  TOK_NOT_BIT,  /* ~ */
  TOK_SHL,      /* << */
  TOK_SHR,      /* >> */
  TOK_INC,      /* ++ */
  TOK_DEC,      /* -- */
  TOK_STRING,
  TOK_NUM_FLOAT,
  TOK_SHL_EQ,
  TOK_SHR_EQ,
  TOK_AND_EQ,
  TOK_OR_EQ,
  TOK_XOR_EQ

} aritlex_token_type;

typedef struct aritlex_token
{
  aritlex_token_type type;

  union
  {
    s32 number_integer;  /* valid if TOK_NUM_INTEGER    */
    f64 number_floating; /* valid if TOK_FLOAT  */
    s8 name[32];         /* valid if TOK_VAR    */
    s8 string[128];      /* valid if TOK_STRING */

  } val;

} aritlex_token;

ARITLEX_API ARITLEX_INLINE u32 aritlex_tokenize(
    s8 *code,
    u32 code_size,
    aritlex_token *tokens,
    u32 tokens_capacity,
    u32 *tokens_size)
{
  if (!code || code_size <= 0 || !tokens || tokens_capacity <= 0 || !tokens_size)
  {
    return 0;
  }

  *tokens_size = 0;

  while (*code)
  {
    char c = *code;

    /* While this switch statement becomes big for digit and character cases the compiler uses much faster instructions than for if/else cases */
    switch (c)
    {
    /* Skip whitespaces */
    case ' ':
    case '\t':
    case '\r':
    case '\n':
    {
      while (aritlex_is_space(*code))
      {
        code++;
      }
      break;
    }
    /* Process digits */
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    {
      char *start = code;
      int is_float = 0;

      /* Hexadecimal: 0x... */
      if (*code == '0' && (*(code + 1) == 'x' || *(code + 1) == 'X'))
      {
        s32 val = 0;
        code += 2;

        while (aritlex_is_hex(*code) || *code == '_')
        {
          if (*code == '_')
          {
            code++;
            continue;
          }
          val = val * 16 + (*code <= '9' ? *code - '0' : (*code <= 'F' ? *code - 'A' + 10 : *code - 'a' + 10));
          code++;
        }

        tokens[*tokens_size].type = TOK_NUM_INTEGER;
        tokens[(*tokens_size)++].val.number_integer = val;
        break;
      }

      /* Binary: 0b... */
      if (*code == '0' && (*(code + 1) == 'b' || *(code + 1) == 'B'))
      {
        s32 val = 0;
        code += 2;

        while (aritlex_is_binary(*code) || *code == '_')
        {
          if (*code == '_')
          {
            code++;
            continue;
          }
          val = (val << 1) | (*code - '0');
          code++;
        }

        tokens[*tokens_size].type = TOK_NUM_INTEGER;
        tokens[(*tokens_size)++].val.number_integer = val;
        break;
      }

      /* Floating-point / decimal */
      while (aritlex_is_digit(*code) || *code == '_')
        code++; /* integer part */

      if (*code == '.')
      {
        is_float = 1;
        code++;
        while (aritlex_is_digit(*code) || *code == '_')
          code++; /* fraction */
      }

      /* Scientific notation */
      if (*code == 'e' || *code == 'E')
      {
        is_float = 1;
        code++;
        if (*code == '+' || *code == '-')
          code++;
        while (aritlex_is_digit(*code) || *code == '_')
          code++; /* exponent digits */
      }

      /* Suffix f/F means float, otherwise double */
      if (*code == 'f' || *code == 'F')
      {
        f32 val = aritlex_strtof(start, (void *)0);
        tokens[*tokens_size].type = TOK_NUM_FLOAT;
        tokens[(*tokens_size)++].val.number_floating = (f64) val;
        code++;
      }
      else if (is_float)
      {
        f64 val = aritlex_strtod(start, (void *)0);
        tokens[*tokens_size].type = TOK_NUM_FLOAT;
        tokens[(*tokens_size)++].val.number_floating = val;
      }
      else
      {
        s32 val = aritlex_strtol(start, (char **)&code, 10);
        tokens[*tokens_size].type = TOK_NUM_INTEGER;
        tokens[(*tokens_size)++].val.number_integer = val;
      }

      break;
    }
    /* Process Names */
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '_':
    {
      int i = 0;

      while (aritlex_is_alpha(*code) || aritlex_is_digit(*code) || (*code) == '_')
      {
        tokens[*tokens_size].val.name[i++] = *code;
        code++;
      }

      tokens[(*tokens_size)++].type = TOK_VAR;

      break;
    }
    case '"':
    {
      int i = 0;
      code++; /* skip opening " */

      while (*code && *code != '"' && i < (int)sizeof(tokens[*tokens_size].val.string) - 1)
      {
        if (*code == '\\')
        { /* handle escape sequences */
          code++;
          switch (*code)
          {
          case 'n':
            tokens[*tokens_size].val.string[i++] = '\n';
            break;
          case 't':
            tokens[*tokens_size].val.string[i++] = '\t';
            break;
          case 'r':
            tokens[*tokens_size].val.string[i++] = '\r';
            break;
          case '"':
            tokens[*tokens_size].val.string[i++] = '"';
            break;
          case '\\':
            tokens[*tokens_size].val.string[i++] = '\\';
            break;
          default:
            tokens[*tokens_size].val.string[i++] = *code;
            break;
          }
        }
        else
        {
          tokens[*tokens_size].val.string[i++] = *code;
        }
        code++;
      }

      tokens[*tokens_size].val.string[i] = '\0'; /* null terminate */

      if (*code == '"')
      {
        code++; /* skip closing " */
      }

      tokens[(*tokens_size)++].type = TOK_STRING;

      break;
    }
    case '+':
    {
      if (*(code + 1) == '+')
      {
        tokens[(*tokens_size)++].type = TOK_INC;
        code += 2;
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_PLUS_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_PLUS;
        code++;
      }
      break;
    }
    case '-':
    {
      if (*(code + 1) == '-')
      {
        tokens[(*tokens_size)++].type = TOK_DEC;
        code += 2;
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_MINUS_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_MINUS;
        code++;
      }
      break;
    }
    case '*':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_MUL_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_MUL;
        code++;
      }
      break;
    }
    case '/':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_DIV_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_DIV;
        code++;
      }
      break;
    }
    case '%':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_MOD_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_MOD;
        code++;
      }
      break;
    }
    case '(':
    {
      tokens[(*tokens_size)++].type = TOK_LPAREN;
      code++;
      break;
    }
    case ')':
    {
      tokens[(*tokens_size)++].type = TOK_RPAREN;
      code++;
      break;
    }
    case '?':
    {
      tokens[(*tokens_size)++].type = TOK_QMARK;
      code++;
      break;
    }
    case ':':
    {
      tokens[(*tokens_size)++].type = TOK_COLON;
      code++;
      break;
    }
    case '=':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_ASSIGN;
        code++;
      }
      break;
    }
    case '!':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_NEQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_NOT;
        code++;
      }
      break;
    }
    case '<':
    {
      if (*(code + 1) == '<')
      {
        if (*(code + 2) == '=')
        {
          tokens[(*tokens_size)++].type = TOK_SHL_EQ;
          code += 3;
        }
        else
        {
          tokens[(*tokens_size)++].type = TOK_SHL;
          code += 2;
        }
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_LE;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_LT;
        code++;
      }
      break;
    }
    case '>':
    {
      if (*(code + 1) == '>')
      {
        if (*(code + 2) == '=')
        {
          tokens[(*tokens_size)++].type = TOK_SHR_EQ;
          code += 3;
        }
        else
        {
          tokens[(*tokens_size)++].type = TOK_SHR;
          code += 2;
        }
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_GE;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_GT;
        code++;
      }
      break;
    }
    case '&':
    {
      if (*(code + 1) == '&')
      {
        tokens[(*tokens_size)++].type = TOK_AND_AND;
        code += 2;
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_AND_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_AND;
        code++;
      }
      break;
    }
    case '|':
    {
      if (*(code + 1) == '|')
      {
        tokens[(*tokens_size)++].type = TOK_OR_OR;
        code += 2;
      }
      else if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_OR_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_OR;
        code++;
      }
      break;
    }
    case '^':
    {
      if (*(code + 1) == '=')
      {
        tokens[(*tokens_size)++].type = TOK_XOR_EQ;
        code += 2;
      }
      else
      {
        tokens[(*tokens_size)++].type = TOK_XOR;
        code++;
      }
      break;
    }
    case '~':
    {
      tokens[(*tokens_size)++].type = TOK_NOT_BIT;
      code++;
      break;
    }
    default:
      code++;
      break;
    }
  }

  tokens[(*tokens_size)++].type = TOK_EOF;

  return 1;
}

#endif /* ARITLEX_H */

/*
   ------------------------------------------------------------------------------
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
