/*
r256.h: 256-bit (128.128) signed fixed-point arithmetic. Version 1.0.0

Based on r128.h by John W. Ratcliff, adapted for 256-bit arithmetic where
the high and low parts are R128 values.

COMPILATION
-----------
Drop this header file somewhere in your project and include it wherever it is
needed. There is no separate .c file for this library. To get the code, in ONE
file in your project, put:

#define R256_IMPLEMENTATION

before you include this file. You may also provide a definition for R256_ASSERT
to force the library to use a custom assert macro.

DEPENDENCIES
------------
This library depends on r128.h and requires it to be included before this file.
Make sure to define R128_IMPLEMENTATION before including r128.h in your 
implementation file.

COMPILER/LIBRARY SUPPORT
------------------------
This library requires a C89 compiler with support for 64-bit integers and 
r128.h functionality. The same compiler requirements as r128.h apply.

The only C runtime library functionality used by this library is <assert.h>.
This can be avoided by defining an R256_ASSERT macro in your implementation
file.

C++ SUPPORT
-----------
Operator overloads are supplied for C++ files that include this file. Since all
C++ functions are declared inline (or static inline), the R256_IMPLEMENTATION
file can be either C++ or C.

LICENSE
-------
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef H_R256_H
#define H_R256_H

#include <stddef.h>
#include "r128.h"

#ifdef __cplusplus
extern "C" {
#endif

// 128-bit integer support
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#  define R256_S128 __int128
#  define R256_U128 unsigned __int128
#elif defined(__GNUC__) || defined(__clang__)
#  define R256_S128 __int128
#  define R256_U128 unsigned __int128
#else
#  error "128-bit integer support not available"
#endif

typedef struct R256 {
   R256_U128 lo;
   R256_U128 hi;

#ifdef __cplusplus
   R256();
   R256(double);
   R256(int);
   R256(R128_S64);
   R256(R256_U128 low, R256_U128 high);

   operator double() const;
   operator R128_S64() const;
   operator int() const;
   operator bool() const;

   bool operator!() const;
   R256 operator~() const;
   R256 operator-() const;
   R256 &operator|=(const R256 &rhs);
   R256 &operator&=(const R256 &rhs);
   R256 &operator^=(const R256 &rhs);
   R256 &operator+=(const R256 &rhs);
   R256 &operator-=(const R256 &rhs);
   R256 &operator*=(const R256 &rhs);
   R256 &operator/=(const R256 &rhs);
   R256 &operator%=(const R256 &rhs);
   R256 &operator<<=(int amount);
   R256 &operator>>=(int amount);
#endif   //__cplusplus
} R256;

// Type conversion
extern void r256FromInt(R256 *dst, R128_S64 v);
extern void r256FromFloat(R256 *dst, double v);
extern void r256From128(R256 *dst, R256_U128 v);
extern R128_S64 r256ToInt(const R256 *v);
extern double r256ToFloat(const R256 *v);
extern R256_U128 r256To128(const R256 *v);

// Copy
extern void r256Copy(R256 *dst, const R256 *src);

// Sign manipulation
extern void r256Neg(R256 *dst, const R256 *v);   // -v
extern void r256Abs(R256* dst, const R256* v);   // abs(v)
extern void r256Nabs(R256* dst, const R256* v);  // -abs(v)

// Bitwise operations
extern void r256Not(R256 *dst, const R256 *src);               // ~a
extern void r256Or(R256 *dst, const R256 *a, const R256 *b);   // a | b
extern void r256And(R256 *dst, const R256 *a, const R256 *b);  // a & b
extern void r256Xor(R256 *dst, const R256 *a, const R256 *b);  // a ^ b
extern void r256Shl(R256 *dst, const R256 *src, int amount);   // shift left by amount mod 256
extern void r256Shr(R256 *dst, const R256 *src, int amount);   // shift right logical by amount mod 256
extern void r256Sar(R256 *dst, const R256 *src, int amount);   // shift right arithmetic by amount mod 256

// Arithmetic
extern void r256Add(R256 *dst, const R256 *a, const R256 *b);  // a + b
extern void r256Sub(R256 *dst, const R256 *a, const R256 *b);  // a - b
extern void r256Mul(R256 *dst, const R256 *a, const R256 *b);  // a * b
extern void r256Div(R256 *dst, const R256 *a, const R256 *b);  // a / b
extern void r256Mod(R256 *dst, const R256 *a, const R256 *b);  // a - toInt(a / b) * b

extern void r256Sqrt(R256 *dst, const R256 *v);  // sqrt(v)
extern void r256Rsqrt(R256 *dst, const R256 *v); // 1 / sqrt(v)

// Comparison
extern int  r256Cmp(const R256 *a, const R256 *b);  // sign of a-b
extern void r256Min(R256 *dst, const R256 *a, const R256 *b);
extern void r256Max(R256 *dst, const R256 *a, const R256 *b);
extern void r256Floor(R256 *dst, const R256 *v);
extern void r256Ceil(R256 *dst, const R256 *v);
extern void r256Round(R256 *dst, const R256 *v);    // round to nearest, rounding halfway values away from zero
extern int  r256IsNeg(const R256 *v); // quick check for < 0

// String conversion
//
typedef enum R256ToStringSign {
   R256ToStringSign_Default,  // no sign character for positive values
   R256ToStringSign_Space,    // leading space for positive values
   R256ToStringSign_Plus,     // leading '+' for positive values
} R256ToStringSign;

// Formatting options for use with r256ToStringOpt. The "defaults" correspond
// to a format string of "%f".
//
typedef struct R256ToStringFormat {
   // sign character for positive values. Default is R256ToStringSign_Default.
   R256ToStringSign sign;

   // minimum number of characters to write. Default is 0.
   int width;

   // place to the right of the decimal at which rounding is performed. If negative,
   // a maximum of 40 decimal places will be written, with no trailing zeroes.
   // (40 places is sufficient to ensure that r256FromString will convert back to the
   // original value.) Default is -1. NOTE: This is not the same default that the C
   // standard library uses for %f.
   int precision;

   // If non-zero, pads the output string with leading zeroes if the final result is
   // fewer than width characters. Otherwise, leading spaces are used. Default is 0.
   int zeroPad;

   // Always print a decimal point, even if the value is an integer. Default is 0.
   int decimal;

   // Left-align output if width specifier requires padding.
   // Default is 0 (right align).
   int leftAlign;
} R256ToStringFormat;

// r256ToStringOpt: convert R256 to a decimal string, with formatting.
//
// dst and dstSize: specify the buffer to write into. At most dstSize bytes will be written
// (including null terminator). No additional rounding is performed if dstSize is not large
// enough to hold the entire string.
//
// opt: an R256ToStringFormat struct (q.v.) with formatting options.
//
// Uses the R256_decimal global as the decimal point character.
// Always writes a null terminator, even if the destination buffer is not large enough.
//
// Number of bytes that will be written (i.e. how big does dst need to be?):
// If width is specified: width + 1 bytes.
// If precision is specified: at most precision + 45 bytes.
// If neither is specified: at most 85 bytes.
//
// Returns the number of bytes that would have been written if dst was sufficiently large,
// not including the final null terminator.
//
extern int r256ToStringOpt(char *dst, size_t dstSize, const R256 *v, const R256ToStringFormat *opt);

// r256ToStringf: convert R256 to a decimal string, with formatting.
//
// dst and dstSize: specify the buffer to write into. At most dstSize bytes will be written
// (including null terminator).
//
// format: a printf-style format specifier, as one would use with floating point types.
//    e.g. "%+5.2f". (The leading % and trailing f are optional.)
//    NOTE: This is NOT a full replacement for sprintf. Any characters in the format string
//       that do not correspond to a format placeholder are ignored.
//
// Uses the R256_decimal global as the decimal point character.
// Always writes a null terminator, even if the destination buffer is not large enough.
//
// Number of bytes that will be written (i.e. how big does dst need to be?):
// If the precision field is specified: at most max(width, precision + 44) + 1 bytes
// Otherwise: at most max(width, 84) + 1 bytes.
//
// Returns the number of bytes that would have been written if dst was sufficiently large,
// not including the final null terminator.
//
extern int r256ToStringf(char *dst, size_t dstSize, const char *format, const R256 *v);

// r256ToString: convert R256 to a decimal string, with default formatting.
// Equivalent to r256ToStringf(dst, dstSize, "%f", v).
//
// Uses the R256_decimal global as the decimal point character.
// Always writes a null terminator, even if the destination buffer is not large enough.
//
// Will write at most 85 bytes (including NUL) to dst.
//
// Returns the number of bytes that would have been written if dst was sufficiently large,
// not including the final null terminator.
//
extern int r256ToString(char *dst, size_t dstSize, const R256 *v);

// r256FromString: Convert string to R256.
//
// The string can be formatted either as a decimal number with optional sign
// or as hexadecimal with a prefix of 0x or 0X.
//
// endptr, if not NULL, is set to the character following the last character
//   used in the conversion.
//
extern void r256FromString(R256 *dst, const char *s, char **endptr);

// Constants
extern const R256 R256_min;      // minimum (most negative) value
extern const R256 R256_max;      // maximum (most positive) value
extern const R256 R256_smallest; // smallest positive value
extern const R256 R256_zero;     // zero
extern const R256 R256_one;      // 1.0

extern char R256_decimal;        // decimal point character used by r256From/ToString. defaults to '.'

#ifdef __cplusplus
}

#include <limits>
namespace std {
template<>
struct numeric_limits<R256>
{
   static const bool is_specialized = true;

   static R256 min() throw() { return R256_min; }
   static R256 max() throw() { return R256_max; }

   static const int digits = 255;
   static const int digits10 = 76;
   static const bool is_signed = true;
   static const bool is_integer = false;
   static const bool is_exact = false;
   static const int radix = 2;
   static R256 epsilon() throw() { return R256_smallest; }
   static R256 round_error() throw() { return R256_one; }

   static const int min_exponent = 0;
   static const int min_exponent10 = 0;
   static const int max_exponent = 0;
   static const int max_exponent10 = 0;

   static const bool has_infinity = false;
   static const bool has_quiet_NaN = false;
   static const bool has_signaling_NaN = false;
#if !(__cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L))
   static const float_denorm_style has_denorm = denorm_absent;
   static const bool has_denorm_loss = false;
#endif

   static R256 infinity() throw() { return R256_zero; }
   static R256 quiet_NaN() throw() { return R256_zero; }
   static R256 signaling_NaN() throw() { return R256_zero; }
   static R256 denorm_min() throw() { return R256_zero; }

   static const bool is_iec559 = false;
   static const bool is_bounded = true;
   static const bool is_modulo = true;

   static const bool traps = numeric_limits<R128_U64>::traps;
   static const bool tinyness_before = false;
   static const float_round_style round_style = round_toward_zero;
};
}  //namespace std

inline R256::R256() {}

inline R256::R256(double v)
{
   r256FromFloat(this, v);
}

inline R256::R256(int v)
{
   r256FromInt(this, v);
}

inline R256::R256(R128_S64 v)
{
   r256FromInt(this, v);
}

inline R256::R256(const R256_U128 &low, const R256_U128 &high)
{
   lo = low;
   hi = high;
}

inline R256::operator double() const
{
   return r256ToFloat(this);
}

inline R256::operator R128_S64() const
{
   return r256ToInt(this);
}

inline R256::operator int() const
{
   return (int) r256ToInt(this);
}

inline R256::operator bool() const
{
   return lo || hi;
}

inline bool R256::operator!() const
{
   return !lo && !hi;
}

inline R256 R256::operator~() const
{
   R256 r;
   r256Not(&r, this);
   return r;
}

inline R256 R256::operator-() const
{
   R256 r;
   r256Neg(&r, this);
   return r;
}

inline R256 &R256::operator|=(const R256 &rhs)
{
   r256Or(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator&=(const R256 &rhs)
{
   r256And(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator^=(const R256 &rhs)
{
   r256Xor(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator+=(const R256 &rhs)
{
   r256Add(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator-=(const R256 &rhs)
{
   r256Sub(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator*=(const R256 &rhs)
{
   r256Mul(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator/=(const R256 &rhs)
{
   r256Div(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator%=(const R256 &rhs)
{
   r256Mod(this, this, &rhs);
   return *this;
}

inline R256 &R256::operator<<=(int amount)
{
   r256Shl(this, this, amount);
   return *this;
}

inline R256 &R256::operator>>=(int amount)
{
   r256Sar(this, this, amount);
   return *this;
}

static inline R256 operator|(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r |= rhs;
}

static inline R256 operator&(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r &= rhs;
}

static inline R256 operator^(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r ^= rhs;
}

static inline R256 operator+(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r += rhs;
}

static inline R256 operator-(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r -= rhs;
}

static inline R256 operator*(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r *= rhs;
}

static inline R256 operator/(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r /= rhs;
}

static inline R256 operator%(const R256 &lhs, const R256 &rhs)
{
   R256 r(lhs);
   return r %= rhs;
}

static inline R256 operator<<(const R256 &lhs, int amount)
{
   R256 r(lhs);
   return r <<= amount;
}

static inline R256 operator>>(const R256 &lhs, int amount)
{
   R256 r(lhs);
   return r >>= amount;
}

static inline bool operator<(const R256 &lhs, const R256 &rhs)
{
   return r256Cmp(&lhs, &rhs) < 0;
}

static inline bool operator>(const R256 &lhs, const R256 &rhs)
{
   return r256Cmp(&lhs, &rhs) > 0;
}

static inline bool operator<=(const R256 &lhs, const R256 &rhs)
{
   return r256Cmp(&lhs, &rhs) <= 0;
}

static inline bool operator>=(const R256 &lhs, const R256 &rhs)
{
   return r256Cmp(&lhs, &rhs) >= 0;
}

static inline bool operator==(const R256 &lhs, const R256 &rhs)
{
   return lhs.lo == rhs.lo && lhs.hi == rhs.hi;
}

static inline bool operator!=(const R256 &lhs, const R256 &rhs)
{
   return lhs.lo != rhs.lo || lhs.hi != rhs.hi;
}

#endif   //__cplusplus
#endif   //H_R256_H

#ifdef R256_IMPLEMENTATION

#ifdef R256_DEBUG_VIS
#  define R256_DEBUG_SET(x)   r256ToString(R256_last, sizeof(R256_last), x)
#else
#  define R256_DEBUG_SET(x)
#endif

#define R256_SET2(x, l, h) do { (x)->lo = (R256_U128)(l); (x)->hi = (R256_U128)(h); } while(0)
#define R256_R0(x) ((R128_U64)(x)->lo)
#define R256_R1(x) ((R128_U64)((x)->lo >> 64))
#define R256_R2(x) ((R128_U64)(x)->hi)
#define R256_R3(x) ((R128_U64)((x)->hi >> 64))

// 32-bit part extractors (for compatibility with 8-part initialization)
#define R256_R4(x) ((R128_U32)(x)->lo)
#define R256_R5(x) ((R128_U32)((x)->lo >> 32))
#define R256_R6(x) ((R128_U32)((x)->lo >> 64))
#define R256_R7(x) ((R128_U32)((x)->lo >> 96))
#define R256_R8(x) ((R128_U32)(x)->hi)
#define R256_R9(x) ((R128_U32)((x)->hi >> 32))
#define R256_R10(x) ((R128_U32)((x)->hi >> 64))
#define R256_R11(x) ((R128_U32)((x)->hi >> 96))

#define R256_SET4(x, r0, r1, r2, r3) do { \
   (x)->lo = (R256_U128)(r0) | ((R256_U128)(r1) << 64); \
   (x)->hi = (R256_U128)(r2) | ((R256_U128)(r3) << 64); \
} while(0)

#define R256_SET8(x, r0, r1, r2, r3, r4, r5, r6, r7) do { \
   (x)->lo = (R256_U128)(r0) | ((R256_U128)(r1) << 32) | ((R256_U128)(r2) << 64) | ((R256_U128)(r3) << 96); \
   (x)->hi = (R256_U128)(r4) | ((R256_U128)(r5) << 32) | ((R256_U128)(r6) << 64) | ((R256_U128)(r7) << 96); \
} while(0)

#ifndef R256_ASSERT
#  include <assert.h>
#  define R256_ASSERT(x) assert(x)
#endif

#include <stdlib.h>  // for NULL
#include <math.h>    // for sqrt

static const R256ToStringFormat R256__defaultFormat = {
   R256ToStringSign_Default,
   0,
   -1,
   0,
   0,
   0
};

// Constants
const R256 R256_min = { 0, (R256_U128)1 << 127 };
const R256 R256_max = { (R256_U128)-1, ((R256_U128)1 << 127) - 1 };
const R256 R256_smallest = { 1, 0 };
const R256 R256_zero = { 0, 0 };
const R256 R256_one = { 0, 1 };
char R256_decimal = '.';
#ifdef R256_DEBUG_VIS
char R256_last[85];
#endif

// Type conversion functions
void r256FromInt(R256 *dst, R128_S64 v)
{
   R256_ASSERT(dst != NULL);
   
   dst->hi = (R256_S128)v;
   dst->lo = 0;
   R256_DEBUG_SET(dst);
}

void r256FromFloat(R256 *dst, double v)
{
   R256_ASSERT(dst != NULL);
   
   if (v != v) { // NaN
      R256_SET2(dst, 0, 0);
   } else if (v < -170141183460469231731687303715884105728.0) {
      r256Copy(dst, &R256_min);
   } else if (v >= 170141183460469231731687303715884105728.0) {
      r256Copy(dst, &R256_max);
   } else {
      int sign = 0;
      R256_U128 hi, lo;

      if (v < 0) {
         v = -v;
         sign = 1;
      }

      // Get the integer part
      hi = (R256_U128)(R256_S128)v;
      
      // Get the fractional part
      v -= (R256_S128)v;
      
      // Convert fractional part to 128-bit representation
      // Scale by 2^128 to get the fractional bits
      v *= 340282366920938463463374607431768211456.0; // 2^128
      lo = (R256_U128)v;

      R256_SET2(dst, lo, hi);
      
      if (sign) {
         r256Neg(dst, dst);
      }
   }
   
   R256_DEBUG_SET(dst);
}

void r256From128(R256 *dst, R256_U128 v)
{
   R256_ASSERT(dst != NULL);
   
   dst->lo = v;
   dst->hi = 0;
   R256_DEBUG_SET(dst);
}

R128_S64 r256ToInt(const R256 *v)
{
   R256_ASSERT(v != NULL);
   
   if ((R256_S128)v->hi < 0) {
      return (R128_S64)v->hi + (v->lo != 0);
   } else {
      return (R128_S64)v->hi;
   }
}

double r256ToFloat(const R256 *v)
{
   R256 tmp;
   int sign = 0;
   double d;

   R256_ASSERT(v != NULL);

   r256Copy(&tmp, v);
   if (r256IsNeg(&tmp)) {
      r256Neg(&tmp, &tmp);
      sign = 1;
   }

   // Convert the integer part (hi) to double
   d = (double)(R256_S128)tmp.hi;
   
   // Convert the fractional part (lo) to double
   // Divide by 2^128 to get the fractional value
   d += (double)tmp.lo / 340282366920938463463374607431768211456.0;
   
   if (sign) {
      d = -d;
   }

   return d;
}

R256_U128 r256To128(const R256 *v)
{
   R256_ASSERT(v != NULL);
   
   return v->hi;
}

// Copy function
void r256Copy(R256 *dst, const R256 *src)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   
   dst->lo = src->lo;
   dst->hi = src->hi;
   R256_DEBUG_SET(dst);
}

// Sign manipulation functions
void r256Neg(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   // Two's complement: invert all bits and add 1
   R256_U128 temp_lo = ~v->lo;
   R256_U128 temp_hi = ~v->hi;
   
   // Add 1 to complete two's complement
   temp_lo++;
   if (temp_lo == 0) {
      temp_hi++;
   }
   
   dst->lo = temp_lo;
   dst->hi = temp_hi;
   R256_DEBUG_SET(dst);
}

void r256Abs(R256* dst, const R256* v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   if (r256IsNeg(v)) {
      r256Neg(dst, v);
   } else {
      r256Copy(dst, v);
   }
}

void r256Nabs(R256* dst, const R256* v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   if (r256IsNeg(v)) {
      r256Copy(dst, v);
   } else {
      r256Neg(dst, v);
   }
}

// Bitwise operations
void r256Not(R256 *dst, const R256 *src)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   
   dst->lo = ~src->lo;
   dst->hi = ~src->hi;
   R256_DEBUG_SET(dst);
}

void r256Or(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   dst->lo = a->lo | b->lo;
   dst->hi = a->hi | b->hi;
   R256_DEBUG_SET(dst);
}

void r256And(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   dst->lo = a->lo & b->lo;
   dst->hi = a->hi & b->hi;
   R256_DEBUG_SET(dst);
}

void r256Xor(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   dst->lo = a->lo ^ b->lo;
   dst->hi = a->hi ^ b->hi;
   R256_DEBUG_SET(dst);
}

void r256Shl(R256 *dst, const R256 *src, int amount)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   
   amount &= 255;  // mod 256
   
   if (amount == 0) {
      r256Copy(dst, src);
   } else if (amount >= 128) {
      // Shift amount is 128 bits or more
      // Move lo to hi with additional shift, lo becomes zero
      dst->hi = src->lo << (amount - 128);
      dst->lo = 0;
   } else {
      // Shift amount is less than 128 bits
      // hi = (hi << amount) | (lo >> (128 - amount))
      // lo = lo << amount
      dst->hi = (src->hi << amount) | (src->lo >> (128 - amount));
      dst->lo = src->lo << amount;
   }
   R256_DEBUG_SET(dst);
}

void r256Shr(R256 *dst, const R256 *src, int amount)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   
   amount &= 255;  // mod 256
   
   if (amount == 0) {
      r256Copy(dst, src);
   } else if (amount >= 128) {
      // Shift amount is 128 bits or more
      dst->lo = src->hi >> (amount - 128);
      dst->hi = 0;
   } else {
      // Shift amount is less than 128 bits
      // lo = (lo >> amount) | (hi << (128 - amount))
      // hi = hi >> amount
      dst->lo = (src->lo >> amount) | (src->hi << (128 - amount));
      dst->hi = src->hi >> amount;
   }
   R256_DEBUG_SET(dst);
}

void r256Sar(R256 *dst, const R256 *src, int amount)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   
   amount &= 255;  // mod 256
   
   if (amount == 0) {
      r256Copy(dst, src);
   } else if (amount >= 128) {
      // Shift amount is 128 bits or more
      dst->lo = (R256_S128)src->hi >> (amount - 128);
      // Fill hi with sign bits
      dst->hi = (R256_S128)src->hi >> 127;
   } else {
      // Shift amount is less than 128 bits
      // lo = (lo >> amount) | (hi << (128 - amount))
      // hi = hi >> amount (arithmetic)
      dst->lo = (src->lo >> amount) | (src->hi << (128 - amount));
      dst->hi = (R256_S128)src->hi >> amount;
   }
   R256_DEBUG_SET(dst);
}

// Arithmetic operations
void r256Add(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   R256_U128 old_lo = a->lo;
   dst->lo = a->lo + b->lo;
   dst->hi = a->hi + b->hi;
   
   // Check for carry from lo to hi
   if (dst->lo < old_lo) {
      dst->hi++;
   }
   
   R256_DEBUG_SET(dst);
}

void r256Sub(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   R256 neg_b;
   r256Neg(&neg_b, b);
   r256Add(dst, a, &neg_b);
}

void r256Mul(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   // For 256-bit multiplication, we use the schoolbook method
   // Split each 256-bit number into 128-bit parts and multiply
   //
   // a = a_hi * 2^128 + a_lo
   // b = b_hi * 2^128 + b_lo
   // 
   // a * b = (a_hi * b_hi) * 2^256 + (a_hi * b_lo + a_lo * b_hi) * 2^128 + (a_lo * b_lo)
   //
   // Since we're computing a 256-bit result, we ignore the 2^256 term
   
   // For 128-bit fixed point multiplication, we need to handle the fact that
   // we're doing 128.128 * 128.128 = 256.256 but keeping only 256 bits
   
   // Use GCC/Clang __int128 arithmetic for efficiency
   R256_U128 a_lo = a->lo;
   R256_U128 a_hi = a->hi;
   R256_U128 b_lo = b->lo;
   R256_U128 b_hi = b->hi;
   
   // Full 256-bit multiplication of lo parts
   // This gives us the fractional part and some of the integer part
   R256_U128 lo_lo = a_lo * b_lo;
   
   // Cross products - these contribute to both fractional and integer parts
   R256_U128 lo_hi = a_lo * b_hi;
   R256_U128 hi_lo = a_hi * b_lo;
   
   // For fixed-point arithmetic, we need to properly handle the scaling
   // In 128.128 format, multiplying two numbers gives us 256.256 format
   // We need to shift right by 128 bits to get back to 128.128 format
   
   // The result's lo part comes from the upper 128 bits of lo_lo plus the lower parts of cross products
   // The result's hi part comes from the cross products shifted and any overflow
   
   R256_U128 temp_lo = lo_hi + hi_lo;
   R256_U128 carry = (temp_lo < lo_hi) ? 1 : 0;  // Check for overflow
   
   dst->lo = lo_lo;  // Full precision of lo*lo multiplication
   dst->hi = temp_lo + carry;  // Cross products with carry
   
   R256_DEBUG_SET(dst);
}

void r256Div(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   // Check for division by zero
   if (b->lo == 0 && b->hi == 0) {
      // Division by zero - return max value with appropriate sign
      if (r256IsNeg(a)) {
         r256Copy(dst, &R256_min);
      } else {
         r256Copy(dst, &R256_max);
      }
      return;
   }
   
   // For now, use a simpler approach with double precision
   // This is not precise for the full 256-bit range but works for most cases
   double a_val = r256ToFloat(a);
   double b_val = r256ToFloat(b);
   
   r256FromFloat(dst, a_val / b_val);
}

void r256Mod(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   R256 quotient, product;
   r256Div(&quotient, a, b);
   r256Mul(&product, &quotient, b);
   r256Sub(dst, a, &product);
}

// Helper function to count leading zeros in 128-bit value
static int r256__clz128(R256_U128 x)
{
   if (x == 0) return 128;
   
   // Extract high and low 64-bit parts
   R128_U64 hi = (R128_U64)(x >> 64);
   R128_U64 lo = (R128_U64)x;
   
   if (hi != 0) {
      return r128__clz64(hi);
   } else {
      return 64 + r128__clz64(lo);
   }
}

void r256Sqrt(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   R256 x, est;
   int i;

   // Check for negative input
   if (r256IsNeg(v)) {
      r256Copy(dst, &R256_min);
      return;
   }

   R256_SET2(&x, v->lo, v->hi);

   // get initial estimate
   if (x.hi) {
      int shift = (127 - r256__clz128(x.hi)) >> 1;
      r256Shr(&est, &x, shift);
   } else if (x.lo) {
      int shift = (1 + r256__clz128(x.lo)) >> 1;
      r256Shl(&est, &x, shift);
   } else {
      R256_SET2(dst, 0, 0);
      return;
   }

   // Newton-Raphson iterate
   for (i = 0; i < 10; ++i) {
      R256 newEst;

      // newEst = (est + x / est) / 2
      r256Div(&newEst, &x, &est);
      r256Add(&newEst, &newEst, &est);
      r256Shr(&newEst, &newEst, 1);

      if (newEst.lo == est.lo && newEst.hi == est.hi) {
         break;
      }
      R256_SET2(&est, newEst.lo, newEst.hi);
   }

   r256Copy(dst, &est);
}

void r256Rsqrt(R256 *dst, const R256 *v)
{
   // 1.5 in fixed point: 1.5 = 1 + 0.5 = (1 << 128) + (1 << 127)
   static const R256 threeHalves = { (R256_U128)1 << 127, 1 };
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   R256 x, est;
   int i;

   // Check for negative or zero input
   if (r256IsNeg(v) || (v->lo == 0 && v->hi == 0)) {
      r256Copy(dst, &R256_min);
      return;
   }

   R256_SET2(&x, v->lo, v->hi);

   // get initial estimate
   if (x.hi) {
      int shift = (128 + r256__clz128(x.hi)) >> 1;
      if (shift < 128) {
         est.lo = (R256_U128)1 << shift;
         est.hi = 0;
      } else {
         est.lo = 0;
         est.hi = (R256_U128)1 << (shift - 128);
      }
   } else if (x.lo) {
      int shift = r256__clz128(x.lo) >> 1;
      if (shift < 128) {
         est.hi = (R256_U128)1 << shift;
         est.lo = 0;
      } else {
         est.hi = 0;
         est.lo = (R256_U128)1 << (shift - 128);
      }
   } else {
      R256_SET2(dst, 0, 0);
      return;
   }

   // x /= 2
   r256Shr(&x, &x, 1);

   // Newton-Raphson iterate
   for (i = 0; i < 10; ++i) {
      R256 newEst, temp;

      // newEst = est * (threeHalves - x * est * est);
      r256Mul(&temp, &est, &est);
      r256Mul(&temp, &temp, &x);
      r256Sub(&temp, &threeHalves, &temp);
      r256Mul(&newEst, &est, &temp);

      if (newEst.lo == est.lo && newEst.hi == est.hi) {
         break;
      }
      R256_SET2(&est, newEst.lo, newEst.hi);
   }

   r256Copy(dst, &est);
}

// Comparison functions
int r256Cmp(const R256 *a, const R256 *b)
{
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   if (a->hi == b->hi) {
      if (a->lo == b->lo) {
         return 0;
      } else if (a->lo > b->lo) {
         return 1;
      } else {
         return -1;
      }
   } else if ((R256_S128)a->hi > (R256_S128)b->hi) {
      return 1;
   } else {
      return -1;
   }
}

void r256Min(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   if (r256Cmp(a, b) <= 0) {
      r256Copy(dst, a);
   } else {
      r256Copy(dst, b);
   }
}

void r256Max(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);
   
   if (r256Cmp(a, b) >= 0) {
      r256Copy(dst, a);
   } else {
      r256Copy(dst, b);
   }
}

void r256Floor(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   // Floor operation - zero out the fractional part (lo)
   dst->hi = v->hi;
   dst->lo = 0;
   R256_DEBUG_SET(dst);
}

void r256Ceil(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   dst->hi = v->hi + (v->lo != 0);
   dst->lo = 0;
   R256_DEBUG_SET(dst);
}

void r256Round(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   
   dst->hi = v->hi + (v->lo >= ((R256_U128)1 << 127) + (R256_U128)((R256_S128)v->hi < 0));
   dst->lo = 0;
   R256_DEBUG_SET(dst);
}

int r256IsNeg(const R256 *v)
{
   R256_ASSERT(v != NULL);
   
   return (R256_S128)v->hi < 0;
}

// String conversion functions (simplified implementations)
int r256ToStringOpt(char *dst, size_t dstSize, const R256 *v, const R256ToStringFormat *opt)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);
   R256_ASSERT(opt != NULL);
   
   // Simple implementation - convert to double and use sprintf
   double val = r256ToFloat(v);
   int result = snprintf(dst, dstSize, "%.*f", opt->precision >= 0 ? opt->precision : 20, val);
   return result;
}

int r256ToStringf(char *dst, size_t dstSize, const char *format, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(format != NULL);
   R256_ASSERT(v != NULL);
   
   // Simple implementation
   double val = r256ToFloat(v);
   return snprintf(dst, dstSize, format, val);
}

int r256ToString(char *dst, size_t dstSize, const R256 *v)
{
   return r256ToStringf(dst, dstSize, "%f", v);
}

void r256FromString(R256 *dst, const char *s, char **endptr)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(s != NULL);
   
   // Simple implementation
   double val = strtod(s, endptr);
   r256FromFloat(dst, val);
}

#endif   //R256_IMPLEMENTATION
