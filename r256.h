/*
r256.h: 256-bit (128.128) signed fixed-point arithmetic. Version 1.6.1

COMPILATION
-----------
Drop this header file somewhere in your project and include it wherever it is
needed. There is no separate .c file for this library. To get the code, in ONE
file in your project, put:

#define R256_IMPLEMENTATION

before you include this file. You may also provide a definition for R256_ASSERT
to force the library to use a custom assert macro.

COMPILER/LIBRARY SUPPORT
------------------------
This library requires a C89 compiler with support for 128-bit integers (__int128). 
If your compiler does not support the __int128 data type, the R256_U128, etc. macros
must be set appropriately. On x86 and x64 targets, Intel intrinsics are used
for speed. If your compiler does not support these intrinsics, you can add
#define R256_STDC_ONLY
in your implementation file before including r256.h.

The only C runtime library functionality used by this library is <assert.h>.
This can be avoided by defining an R256_ASSERT macro in your implementation
file. Since this library uses 128-bit arithmetic, this may implicitly add a
runtime library dependency on 32-bit platforms.

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

// 128-bit integer support
// If your compiler does not have __int128, add appropriate defines for these macros.
#if defined(_MSC_VER) && (_MSC_VER < 1600)
#  define R256_S64 __int64
#  define R256_U64 unsigned __int64
#  define R256_S128 __int128
#  define R256_U128 unsigned __int128
#  define R256_LIT_S64(x) x##i64
#  define R256_LIT_U64(x) x##ui64
#else
#  include <stdint.h>
#  define R256_S32 int32_t
#  define R256_U32 uint32_t
#  define R256_S64 int64_t
#  define R256_U64 uint64_t
#  define R256_S128 __int128
#  define R256_U128 unsigned __int128
#  define R256_LIT_S64(x) x##ll
#  define R256_LIT_U64(x) x##ull
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct R256 {
   R256_U128 lo;
   R256_U128 hi;

#ifdef __cplusplus
   R256();
   R256(double);
   R256(int);
   R256(R256_S64);
   R256(R256_U128 low, R256_U128 high);

   operator double() const;
   operator R256_S64() const;
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
extern void r256FromInt(R256 *dst, R256_S64 v);
extern void r256FromFloat(R256 *dst, double v);
extern R256_S64 r256ToInt(const R256 *v);
extern double r256ToFloat(const R256 *v);

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
   // a maximum of 39 decimal places will be written, with no trailing zeroes.
   // (39 places is sufficient to ensure that r256FromString will convert back to the
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
// If precision is specified: at most precision + 42 bytes.
// If neither is specified: at most 82 bytes.
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
// If the precision field is specified: at most max(width, precision + 41) + 1 bytes
// Otherwise: at most max(width, 81) + 1 bytes.
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
// Will write at most 82 bytes (including NUL) to dst.
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
   static const int digits10 = 77;
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

   static const bool traps = numeric_limits<R256_U128>::traps;
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

inline R256::R256(R256_S64 v)
{
   r256FromInt(this, v);
}

inline R256::R256(R256_U128 low, R256_U128 high)
{
   lo = low;
   hi = high;
}

inline R256::operator double() const
{
   return r256ToFloat(this);
}

inline R256::operator R256_S64() const
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
#define R256_R0(x) ((R256_U64)(x)->lo)
#define R256_R1(x) ((R256_U64)((x)->lo >> 64))
#define R256_R2(x) ((R256_U64)(x)->hi)
#define R256_R3(x) ((R256_U64)((x)->hi >> 64))
#define R256_SET4(x, r0, r1, r2, r3) do { (x)->lo = (R256_U128)(r0) | ((R256_U128)(r1) << 64); \
      (x)->hi = (R256_U128)(r2) | ((R256_U128)(r3) << 64); } while(0)

#if defined(_M_X64)
#  define R256_INTEL 1
#  define R256_64BIT 1
#  ifndef R256_STDC_ONLY
#     include <intrin.h>
#  endif
#elif defined(__x86_64__)
#  define R256_INTEL 1
#  define R256_64BIT 1
#  ifndef R256_STDC_ONLY
#     include <x86intrin.h>
#  endif
#elif defined(_M_IX86)
#  define R256_INTEL 1
#  ifndef R256_STDC_ONLY
#     include <intrin.h>
#  endif
#elif defined(__i386__)
#  define R256_INTEL 1
#  ifndef R256_STDC_ONLY
#     include <x86intrin.h>
#  endif
#elif defined(_M_ARM)
#  ifndef R256_STDC_ONLY
#     include <intrin.h>
#  endif
#elif defined(_M_ARM64)
#  define R256_64BIT 1
#  ifndef R256_STDC_ONLY
#     include <intrin.h>
#  endif
#elif defined(__aarch64__)
#  define R256_64BIT 1
#endif

#ifndef R256_INTEL
#  define R256_INTEL 0
#endif

#ifndef R256_64BIT
#  define R256_64BIT 0
#endif

#ifndef R256_ASSERT
#  include <assert.h>
#  define R256_ASSERT(x) assert(x)
#endif

#include <stdlib.h>  // for NULL

static const R256ToStringFormat R256__defaultFormat = {
   R256ToStringSign_Default,
   0,
   -1,
   0,
   0,
   0
};

const R256 R256_min = { 0, (R256_U128)1 << 127 };
const R256 R256_max = { ~(R256_U128)0, ((R256_U128)1 << 127) - 1 };
const R256 R256_smallest = { 1, 0 };
const R256 R256_zero = { 0, 0 };
const R256 R256_one = { 0, 1 };
char R256_decimal = '.';
#ifdef R256_DEBUG_VIS
char R256_last[82];
#endif

static int r256__clz128(R256_U128 x)
{
#if defined(R256_STDC_ONLY)
   R256_U128 n = 128, y;
   y = x >> 64; if (y) { n -= 64; x = y; }
   y = x >> 32; if (y) { n -= 32; x = y; }
   y = x >> 16; if (y) { n -= 16; x = y; }
   y = x >>  8; if (y) { n -=  8; x = y; }
   y = x >>  4; if (y) { n -=  4; x = y; }
   y = x >>  2; if (y) { n -=  2; x = y; }
   y = x >>  1; if (y) { n -=  1; x = y; }
   return (int)(n - x);
#elif defined(__GNUC__) && !defined(__clang__)
   return x ? __builtin_clzll((unsigned long long)(x >> 64)) + (x >> 64 ? 0 : __builtin_clzll((unsigned long long)x) + 64) : 128;
#else
   R256_U64 hi = (R256_U64)(x >> 64);
   R256_U64 lo = (R256_U64)x;
   if (hi) {
      return __builtin_clzll(hi);
   } else if (lo) {
      return 64 + __builtin_clzll(lo);
   } else {
      return 128;
   }
#endif
}

#if !R256_64BIT
// 64*64->128
static R256_U128 r256__umul128(R256_U64 a, R256_U64 b)
{
#  if defined(_M_IX86) && !defined(R256_STDC_ONLY) && !defined(__MINGW32__)
   return __emulu(a, b);
#  elif defined(_M_ARM) && !defined(R256_STDC_ONLY) && !defined(__MINGW32__)
   return _arm_umull(a, b);
#  else
   return a * (R256_U128)b;
#  endif
}

// 128/64->64
static R256_U64 r256__udiv128(R256_U64 nlo, R256_U64 nhi, R256_U64 d, R256_U64 *rem)
{
#  if defined(_M_IX86) && (_MSC_VER >= 1920) && !defined(R256_STDC_ONLY)
   unsigned __int128 n = ((unsigned __int128)nhi << 64) | nlo;
   return _udiv128(n, d, rem);
#  elif defined(_M_IX86) && !defined(R256_STDC_ONLY) && !defined(__MINGW32__)
   __asm {
      mov rax, nlo
      mov rdx, nhi
      div d
      mov rcx, rem
      mov qword ptr [rcx], rdx
   }
#  elif defined(__i386__) && !defined(R256_STDC_ONLY)
   R256_U64 q, r;
   __asm("divq %4"
      : "=a"(q), "=d"(r)
      : "a"(nlo), "d"(nhi), "X"(d));
   *rem = r;
   return q;
#  else
   R256_U128 n128 = ((R256_U128)nhi << 64) | nlo;
   *rem = (R256_U64)(n128 % d);
   return (R256_U64)(n128 / d);
#  endif
}
#elif defined(R256_STDC_ONLY) || !R256_INTEL
#define r256__umul128(a, b) ((a) * (R256_U128)(b))
static R256_U64 r256__udiv128(R256_U64 nlo, R256_U64 nhi, R256_U64 d, R256_U64 *rem)
{
   R256_U128 n128 = ((R256_U128)nhi << 64) | nlo;
   *rem = (R256_U64)(n128 % d);
   return (R256_U64)(n128 / d);
}
#endif   //!R256_64BIT

static void r256__neg(R256 *dst, const R256 *src)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);

   if (src->lo) {
      dst->lo = ~src->lo + 1;
      dst->hi = ~src->hi;
   } else {
      dst->lo = 0;
      dst->hi = ~src->hi + 1;
   }
}

// 128*128->256
static void r256__umul256(R256 *dst, R256_U128 a, R256_U128 b)
{
   R256_U64 alo = (R256_U64)a;
   R256_U64 ahi = (R256_U64)(a >> 64);
   R256_U64 blo = (R256_U64)b;
   R256_U64 bhi = (R256_U64)(b >> 64);
   
   R256_U128 p0, p1, p2, p3;
   R256_U128 carry, lo, hi;

   p0 = (R256_U128)alo * blo;
   p1 = (R256_U128)alo * bhi;
   p2 = (R256_U128)ahi * blo;
   p3 = (R256_U128)ahi * bhi;

   carry = ((R256_U128)(R256_U64)p1 + (R256_U128)(R256_U64)p2 + (p0 >> 64)) >> 64;
   
   lo = p0 + ((p1 + p2) << 64);
   hi = p3 + ((R256_U64)(p1 >> 64) + (R256_U64)(p2 >> 64)) + carry;

   R256_SET2(dst, lo, hi);
}

// 256/128->128
static R256_U128 r256__udiv256(R256_U128 nlo, R256_U128 nhi, R256_U128 d, R256_U128 *rem)
{
   R256_U128 tmp;
   R256_U64 d0, d1;
   R256_U64 n3, n2, n1, n0;
   R256_U64 q0, q1;
   R256_U64 r;
   int shift;

   R256_ASSERT(d != 0);    //division by zero
   R256_ASSERT(nhi < d);   //overflow

   // normalize
   shift = r256__clz128(d);

   if (shift) {
      R256 tmp256;
      R256_SET2(&tmp256, nlo, nhi);
      r256Shl(&tmp256, &tmp256, shift);
      n3 = R256_R3(&tmp256);
      n2 = R256_R2(&tmp256);
      n1 = R256_R1(&tmp256);
      n0 = R256_R0(&tmp256);
      d <<= shift;
   } else {
      n3 = (R256_U64)(nhi >> 64);
      n2 = (R256_U64)nhi;
      n1 = (R256_U64)(nlo >> 64);
      n0 = (R256_U64)nlo;
   }

   d1 = (R256_U64)(d >> 64);
   d0 = (R256_U64)d;

   // first digit
   R256_ASSERT(n3 <= d1);
   if (n3 < d1) {
      R256_U128 dividend = ((R256_U128)n3 << 64) + n2;
      q1 = (R256_U64)(dividend / d1);
      r = (R256_U64)(dividend % d1);
   } else {
      q1 = ~(R256_U64)0;
      r = n2 + d1;
   }
refine1:
   if (((R256_U128)q1 * d0) > (((R256_U128)r << 64) + n1)) {
      --q1;
      if (r < ~d1 + 1) {
         r += d1;
         goto refine1;
      }
   }

   tmp = ((R256_U128)n2 << 64) + n1 - (((R256_U128)q1 * d0) + (((R256_U128)q1 * d1) << 64));
   n2 = (R256_U64)(tmp >> 64);
   n1 = (R256_U64)tmp;

   // second digit
   R256_ASSERT(n2 <= d1);
   if (n2 < d1) {
      R256_U128 dividend = ((R256_U128)n2 << 64) + n1;
      q0 = (R256_U64)(dividend / d1);
      r = (R256_U64)(dividend % d1);
   } else {
      q0 = ~(R256_U64)0;
      r = n1 + d1;
   }
refine0:
   if (((R256_U128)q0 * d0) > (((R256_U128)r << 64) + n0)) {
      --q0;
      if (r < ~d1 + 1) {
         r += d1;
         goto refine0;
      }
   }

   tmp = ((R256_U128)n1 << 64) + n0 - (((R256_U128)q0 * d0) + (((R256_U128)q0 * d1) << 64));
   n1 = (R256_U64)(tmp >> 64);
   n0 = (R256_U64)tmp;

   *rem = (((R256_U128)n1 << 64) + n0) >> shift;
   return ((R256_U128)q1 << 64) + q0;
}

static int r256__ucmp(const R256 *a, const R256 *b)
{
   if (a->hi != b->hi) {
      if (a->hi > b->hi) {
         return 1;
      } else {
         return -1;
      }
   } else {
      if (a->lo == b->lo) {
         return 0;
      } else if (a->lo > b->lo) {
         return 1;
      } else {
         return -1;
      }
   }
}

static void r256__umul(R256 *dst, const R256 *a, const R256 *b)
{
   R256 p0, p1, p2, p3, round;

   r256__umul256(&p0, a->lo, b->lo);
   round.hi = 0; round.lo = p0.lo >> 127;
   p0.lo = p0.hi; p0.hi = 0; //r256Shr(&p0, &p0, 128);
   r256Add(&p0, &p0, &round);

   r256__umul256(&p1, a->hi, b->lo);
   r256Add(&p0, &p0, &p1);

   r256__umul256(&p2, a->lo, b->hi);
   r256Add(&p0, &p0, &p2);

   r256__umul256(&p3, a->hi, b->hi);
   p3.hi = p3.lo; p3.lo = 0; //r256Shl(&p3, &p3, 128);
   r256Add(&p0, &p0, &p3);

   R256_SET2(dst, p0.lo, p0.hi);
}

// Shift d left until the high bit is set, and shift n left by the same amount.
// returns non-zero on overflow.
static int r256__norm(R256 *n, R256 *d, R256_U128 *n2)
{
   R256_U128 d0, d1;
   R256_U128 n0, n1;
   int shift;

   d1 = d->hi;
   d0 = d->lo;
   n1 = n->hi;
   n0 = n->lo;

   if (d1) {
      shift = r256__clz128(d1);
      if (shift) {
         d1 = (d1 << shift) | (d0 >> (128 - shift));
         d0 = d0 << shift;
         *n2 = n1 >> (128 - shift);
         n1 = (n1 << shift) | (n0 >> (128 - shift));
         n0 = n0 << shift;
      } else {
         *n2 = 0;
      }
   } else {
      shift = r256__clz128(d0);
      if (r256__clz128(n1) <= shift) {
         return 1; // overflow
      }

      if (shift) {
         d1 = d0 << shift;
         d0 = 0;
         *n2 = (n1 << shift) | (n0 >> (128 - shift));
         n1 = n0 << shift;
         n0 = 0;
      } else {
         d1 = d0;
         d0 = 0;
         *n2 = n1;
         n1 = n0;
         n0 = 0;
      }
   }

   R256_SET2(n, n0, n1);
   R256_SET2(d, d0, d1);
   return 0;
}

static void r256__udiv(R256 *quotient, const R256 *dividend, const R256 *divisor)
{
   R256 tmp;
   R256_U128 d0, d1;
   R256_U128 n1, n2, n3;
   R256 q;

   R256_ASSERT(dividend != NULL);
   R256_ASSERT(divisor != NULL);
   R256_ASSERT(quotient != NULL);
   R256_ASSERT(divisor->hi != 0 || divisor->lo != 0);  // divide by zero

   // scale dividend and normalize
   {
      R256 n, d;
      R256_SET2(&n, dividend->lo, dividend->hi);
      R256_SET2(&d, divisor->lo, divisor->hi);
      if (r256__norm(&n, &d, &n3)) {
         R256_SET2(quotient, R256_max.lo, R256_max.hi);
         return;
      }

      d1 = d.hi;
      d0 = d.lo;
      n2 = n.hi;
      n1 = n.lo;
   }

   // first digit
   R256_ASSERT(n3 <= d1);
   {
      R256 t0, t1;
      t0.lo = n1;
      if (n3 < d1) {
         q.hi = r256__udiv256(n2, n3, d1, &t0.hi);
      } else {
         q.hi = ~(R256_U128)0;
         t0.hi = n2 + d1;
      }

refine1:
      r256__umul256(&t1, q.hi, d0);
      if (r256__ucmp(&t1, &t0) > 0) {
         --q.hi;
         if (t0.hi < ~d1 + 1) {
            t0.hi += d1;
            goto refine1;
         }
      }
   }

   {
      R256 t0, t1, t2;
      t0.hi = n2;
      t0.lo = n1;

      r256__umul256(&t1, q.hi, d0);
      r256__umul256(&t2, q.hi, d1);

      t2.hi = t2.lo; t2.lo = 0;  //r256Shl(&t2, &t2, 128);
      r256Add(&tmp, &t1, &t2);
      r256Sub(&tmp, &t0, &tmp);
   }
   n2 = tmp.hi;
   n1 = tmp.lo;

   // second digit
   R256_ASSERT(n2 <= d1);
   {
      R256 t0, t1;
      t0.lo = 0;
      if (n2 < d1) {
         q.lo = r256__udiv256(n1, n2, d1, &t0.hi);
      } else {
         q.lo = ~(R256_U128)0;
         t0.hi = n1 + d1;
      }

   refine0:
      r256__umul256(&t1, q.lo, d0);
      if (r256__ucmp(&t1, &t0) > 0) {
         --q.lo;
         if (t0.hi < ~d1 + 1) {
            t0.hi += d1;
            goto refine0;
         }
      }
   }

   R256_SET2(quotient, q.lo, q.hi);
}

static R256_U128 r256__umod(R256 *n, R256 *d)
{
   R256_U128 d0, d1;
   R256_U128 n3, n2, n1;
   R256_U128 q;

   R256_ASSERT(d != NULL);
   R256_ASSERT(n != NULL);
   R256_ASSERT(d->hi != 0 || d->lo != 0);  // divide by zero

   if (r256__norm(n, d, &n3)) {
      return ~(R256_U128)0;
   }

   d1 = d->hi;
   d0 = d->lo;
   n2 = n->hi;
   n1 = n->lo;

   R256_ASSERT(n3 < d1);
   {
      R256 t0, t1;
      t0.lo = n1;
      q = r256__udiv256(n2, n3, d1, &t0.hi);

   refine1:
      r256__umul256(&t1, q, d0);
      if (r256__ucmp(&t1, &t0) > 0) {
         --q;
         if (t0.hi < ~d1 + 1) {
            t0.hi += d1;
            goto refine1;
         }
      }
   }

   return q;
}

static int r256__format(char *dst, size_t dstSize, const R256 *v, const R256ToStringFormat *format)
{
   char buf[256];
   R256 tmp;
   R256_U128 whole;
   char *cursor, *decimal, *dstp = dst;
   int sign = 0;
   int fullPrecision = 1;
   int width, precision;
   int padCnt, trail = 0;

   R256_ASSERT(dst != NULL && dstSize > 0);
   R256_ASSERT(v != NULL);
   R256_ASSERT(format != NULL);

   --dstSize;

   R256_SET2(&tmp, v->lo, v->hi);
   if (r256IsNeg(&tmp)) {
      r256__neg(&tmp, &tmp);
      sign = 1;
   }

   width = format->width;
   if (width < 0) {
      width = 0;
   }

   precision = format->precision;
   if (precision < 0) {
      // print a maximum of 39 digits
      fullPrecision = 0;
      precision = 39;
   } else if (precision > sizeof(buf) - 41) {
      trail = precision - (sizeof(buf) - 41);
      precision -= trail;
   }

   whole = tmp.hi;
   decimal = cursor = buf;

   // fractional part first in case a carry into the whole part is required
   if (tmp.lo || format->decimal) {
      while (tmp.lo || (fullPrecision && precision)) {
         if ((int)(cursor - buf) == precision) {
            if ((R256_S128)tmp.lo < 0) {
               // round up, propagate carry backwards
               char *c;
               for (c = cursor - 1; c >= buf; --c) {
                  char d = ++*c;
                  if (d <= '9') {
                     goto endfrac;
                  } else {
                     *c = '0';
                  }
               }

               // carry out into the whole part
               whole++;
            }

            break;
         }

         r256__umul256(&tmp, tmp.lo, 10);
         *cursor++ = (char)tmp.hi + '0';
      }

   endfrac:
      if (format->decimal || precision) {
         decimal = cursor;
         *cursor++ = R256_decimal;
      }
   }

   // whole part
   do {
      char digit = (char)(whole % 10);
      whole /= 10;
      *cursor++ = digit + '0';
   } while (whole);

#define R256__WRITE(c) do { if (dstp < dst + dstSize) *dstp = c; ++dstp; } while(0)

   padCnt = width - (int)(cursor - buf) - 1;

   // left padding
   if (!format->leftAlign) {
      char padChar = format->zeroPad ? '0' : ' ';
      if (format->zeroPad) {
         if (sign) {
            R256__WRITE('-');
         } else if (format->sign == R256ToStringSign_Plus) {
            R256__WRITE('+');
         } else if (format->sign == R256ToStringSign_Space) {
            R256__WRITE(' ');
         } else {
            ++padCnt;
         }
      }

      for (; padCnt > 0; --padCnt) {
         R256__WRITE(padChar);
      }
   }

   if (format->leftAlign || !format->zeroPad) {
      if (sign) {
         R256__WRITE('-');
      } else if (format->sign == R256ToStringSign_Plus) {
         R256__WRITE('+');
      } else if (format->sign == R256ToStringSign_Space) {
         R256__WRITE(' ');
      } else {
         ++padCnt;
      }
   }

   {
      char *i;

      // reverse the whole part
      for (i = cursor - 1; i >= decimal; --i) {
         R256__WRITE(*i);
      }

      // copy the fractional part
      for (i = buf; i < decimal; ++i) {
         R256__WRITE(*i);
      }
   }

   // right padding
   if (format->leftAlign) {
      char padChar = format->zeroPad ? '0' : ' ';
      for (; padCnt > 0; --padCnt) {
         R256__WRITE(padChar);
      }
   }

   // trailing zeroes for very large precision
   while (trail--) {
      R256__WRITE('0');
   }

#undef R256__WRITE

   if (dstp <= dst + dstSize) {
      *dstp = '\0';
   } else {
      dst[dstSize] = '\0';
   }
   return (int)(dstp - dst);
}

void r256FromInt(R256 *dst, R256_S64 v)
{
   R256_ASSERT(dst != NULL);
   dst->lo = 0;
   dst->hi = (R256_U128)v;
   R256_DEBUG_SET(dst);
}

void r256FromFloat(R256 *dst, double v)
{
   R256_ASSERT(dst != NULL);

   if (v < -1.7014118346046923e+38) {
      r256Copy(dst, &R256_min);
   } else if (v >= 1.7014118346046923e+38) {
      r256Copy(dst, &R256_max);
   } else {
      R256 r;
      int sign = 0;

      if (v < 0) {
         v = -v;
         sign = 1;
      }

      r.hi = (R256_U128)v;
      v -= (double)(R256_U128)v;
      r.lo = (R256_U128)(v * 340282366920938463463374607431768211456.0);

      if (sign) {
         r256__neg(&r, &r);
      }

      r256Copy(dst, &r);
   }
}

void r256FromString(R256 *dst, const char *s, char **endptr)
{
   R256_U128 lo = 0, hi = 0;
   R256_U128 base = 10;

   int sign = 0;

   R256_ASSERT(dst != NULL);
   R256_ASSERT(s != NULL);

   R256_SET2(dst, 0, 0);

   // consume whitespace
   for (;;) {
      if (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n' || *s == '\v') {
         ++s;
      } else {
         break;
      }
   }

   // sign
   if (*s == '-') {
      sign = 1;
      ++s;
   } else if (*s == '+') {
      ++s;
   }

   // parse base prefix
   if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
      base = 16;
      s += 2;
   }

   // whole part
   for (;; ++s) {
      R256_U128 digit;

      if ('0' <= *s && *s <= '9') {
         digit = *s - '0';
      } else if (base == 16 && 'a' <= *s && *s <= 'f') {
         digit = *s - 'a' + 10;
      } else if (base == 16 && 'A' <= *s && *s <= 'F') {
         digit = *s - 'A' + 10;
      } else {
         break;
      }

      hi = hi * base + digit;
   }

   // fractional part
   if (*s == R256_decimal) {
      const char *exp = ++s;

      // find the last digit and work backwards
      for (;; ++s) {
         if ('0' <= *s && *s <= '9') {
         } else if (base == 16 && ('a' <= *s && *s <= 'f')) {
         } else if (base == 16 && ('A' <= *s && *s <= 'F')) {
         } else {
            break;
         }
      }

      for (const char *c = s - 1; c >= exp; --c) {
         R256_U128 digit, unused;

         if ('0' <= *c && *c <= '9') {
            digit = *c - '0';
         } else if ('a' <= *c && *c <= 'f') {
            digit = *c - 'a' + 10;
         } else {
            digit = *c - 'A' + 10;
         }

         lo = r256__udiv256(lo, digit, base, &unused);
      }
   }

   R256_SET2(dst, lo, hi);
   if (sign) {
      r256__neg(dst, dst);
   }

   if (endptr) {
      *endptr = (char *) s;
   }
}

R256_S64 r256ToInt(const R256 *v)
{
   R256_ASSERT(v != NULL);
   if ((R256_S128)v->hi < 0) {
      return (R256_S64)v->hi + (v->lo != 0);
   } else {
      return (R256_S64)v->hi;
   }
}

double r256ToFloat(const R256 *v)
{
   R256 tmp;
   int sign = 0;
   double d;

   R256_ASSERT(v != NULL);

   R256_SET2(&tmp, v->lo, v->hi);
   if (r256IsNeg(&tmp)) {
      r256__neg(&tmp, &tmp);
      sign = 1;
   }

   d = (double)tmp.hi + (double)tmp.lo * (1.0 / 340282366920938463463374607431768211456.0);
   if (sign) {
      d = -d;
   }

   return d;
}

int r256ToStringOpt(char *dst, size_t dstSize, const R256 *v, const R256ToStringFormat *opt)
{
   return r256__format(dst, dstSize, v, opt);
}

int r256ToStringf(char *dst, size_t dstSize, const char *format, const R256 *v)
{
   R256ToStringFormat opts;

   R256_ASSERT(dst != NULL && dstSize);
   R256_ASSERT(format != NULL);
   R256_ASSERT(v != NULL);

   opts.sign = R256__defaultFormat.sign;
   opts.precision = R256__defaultFormat.precision;
   opts.zeroPad = R256__defaultFormat.zeroPad;
   opts.decimal = R256__defaultFormat.decimal;
   opts.leftAlign = R256__defaultFormat.leftAlign;

   if (*format == '%') {
      ++format;
   }

   // flags field
   for (;; ++format) {
      if (*format == ' ' && opts.sign != R256ToStringSign_Plus) {
         opts.sign = R256ToStringSign_Space;
      } else if (*format == '+') {
         opts.sign = R256ToStringSign_Plus;
      } else if (*format == '0') {
         opts.zeroPad = 1;
      } else if (*format == '-') {
         opts.leftAlign = 1;
      } else if (*format == '#') {
         opts.decimal = 1;
      } else {
         break;
      }
   }

   // width field
   opts.width = 0;
   for (;;) {
      if ('0' <= *format && *format <= '9') {
         opts.width = opts.width * 10 + *format++ - '0';
      } else {
         break;
      }
   }

   // precision field
   if (*format == '.') {
      opts.precision = 0;
      ++format;
      for (;;) {
         if ('0' <= *format && *format <= '9') {
            opts.precision = opts.precision * 10 + *format++ - '0';
         } else {
            break;
         }
      }
   }

   return r256__format(dst, dstSize, v, &opts);
}

int r256ToString(char *dst, size_t dstSize, const R256 *v)
{
   return r256__format(dst, dstSize, v, &R256__defaultFormat);
}

void r256Copy(R256 *dst, const R256 *src)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);
   dst->lo = src->lo;
   dst->hi = src->hi;
   R256_DEBUG_SET(dst);
}

void r256Neg(R256 *dst, const R256 *v)
{
   r256__neg(dst, v);
   R256_DEBUG_SET(dst);
}

void r256Abs(R256* dst, const R256* v)
{
    R256 sign, inv;

    R256_ASSERT(dst != NULL);
    R256_ASSERT(v != NULL);

    sign.lo = sign.hi = (R256_U128)(((R256_S128)v->hi) >> 127);
    inv.lo = v->lo ^ sign.lo;
    inv.hi = v->hi ^ sign.hi;

    r256Sub(dst, &inv, &sign);
}

void r256Nabs(R256* dst, const R256* v)
{
    R256 sign, inv;

    R256_ASSERT(dst != NULL);
    R256_ASSERT(v != NULL);

    sign.lo = sign.hi = (R256_U128)(((R256_S128)v->hi) >> 127);
    inv.lo = v->lo ^ sign.lo;
    inv.hi = v->hi ^ sign.hi;

    r256Sub(dst, &sign, &inv);
}

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
   R256_U128 r[2];

   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);

   r[0] = src->lo;
   r[1] = src->hi;

   amount &= 255;
   if (amount >= 128) {
      r[1] = r[0] << (amount - 128);
      r[0] = 0;
   } else if (amount) {
      r[1] = (r[1] << amount) | (r[0] >> (128 - amount));
      r[0] = r[0] << amount;
   }

   dst->lo = r[0];
   dst->hi = r[1];
   R256_DEBUG_SET(dst);
}

void r256Shr(R256 *dst, const R256 *src, int amount)
{
   R256_U128 r[2];

   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);

   r[0] = src->lo;
   r[1] = src->hi;

   amount &= 255;
   if (amount >= 128) {
      r[0] = r[1] >> (amount - 128);
      r[1] = 0;
   } else if (amount) {
      r[0] = (r[0] >> amount) | (r[1] << (128 - amount));
      r[1] = r[1] >> amount;
   }

   dst->lo = r[0];
   dst->hi = r[1];
   R256_DEBUG_SET(dst);
}

void r256Sar(R256 *dst, const R256 *src, int amount)
{
   R256_U128 r[2];

   R256_ASSERT(dst != NULL);
   R256_ASSERT(src != NULL);

   r[0] = src->lo;
   r[1] = src->hi;

   amount &= 255;
   if (amount >= 128) {
      r[0] = (R256_U128)((R256_S128)r[1] >> (amount - 128));
      r[1] = (R256_U128)((R256_S128)r[1] >> 127);
   } else if (amount) {
      r[0] = (r[0] >> amount) | (R256_U128)((R256_S128)r[1] << (128 - amount));
      r[1] = (R256_U128)((R256_S128)r[1] >> amount);
   }

   dst->lo = r[0];
   dst->hi = r[1];
   R256_DEBUG_SET(dst);
}

void r256Add(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   R256_U128 r = a->lo + b->lo;
   int carry = r < a->lo;
   dst->lo = r;
   dst->hi = a->hi + b->hi + carry;

   R256_DEBUG_SET(dst);
}

void r256Sub(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   R256_U128 r = a->lo - b->lo;
   int borrow = r > a->lo;
   dst->lo = r;
   dst->hi = a->hi - b->hi - borrow;

   R256_DEBUG_SET(dst);
}

void r256Mul(R256 *dst, const R256 *a, const R256 *b)
{
   int sign = 0;
   R256 ta, tb, tc;

   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   R256_SET2(&ta, a->lo, a->hi);
   R256_SET2(&tb, b->lo, b->hi);

   if (r256IsNeg(&ta)) {
      r256__neg(&ta, &ta);
      sign = !sign;
   }
   if (r256IsNeg(&tb)) {
      r256__neg(&tb, &tb);
      sign = !sign;
   }

   r256__umul(&tc, &ta, &tb);
   if (sign) {
      r256__neg(&tc, &tc);
   }

   r256Copy(dst, &tc);
}

void r256Div(R256 *dst, const R256 *a, const R256 *b)
{
   int sign = 0;
   R256 tn, td, tq;

   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   R256_SET2(&tn, a->lo, a->hi);
   R256_SET2(&td, b->lo, b->hi);

   if (r256IsNeg(&tn)) {
      r256__neg(&tn, &tn);
      sign = !sign;
   }

   if (td.lo == 0 && td.hi == 0) {
      // divide by zero
      if (sign) {
         r256Copy(dst, &R256_min);
      } else {
         r256Copy(dst, &R256_max);
      }
      return;
   } else if (r256IsNeg(&td)) {
      r256__neg(&td, &td);
      sign = !sign;
   }

   r256__udiv(&tq, &tn, &td);

   if (sign) {
      r256__neg(&tq, &tq);
   }

   r256Copy(dst, &tq);
}

void r256Mod(R256 *dst, const R256 *a, const R256 *b)
{
   int sign = 0;
   R256 tn, td, tq;

   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   R256_SET2(&tn, a->lo, a->hi);
   R256_SET2(&td, b->lo, b->hi);

   if (r256IsNeg(&tn)) {
      r256__neg(&tn, &tn);
      sign = !sign;
   }

   if (td.lo == 0 && td.hi == 0) {
      // divide by zero
      if (sign) {
         r256Copy(dst, &R256_min);
      } else {
         r256Copy(dst, &R256_max);
      }
      return;
   } else if (r256IsNeg(&td)) {
      r256__neg(&td, &td);
      sign = !sign;
   }

   tq.hi = r256__umod(&tn, &td);
   tq.lo = 0;

   if (sign) {
      tq.hi = ~tq.hi + 1;
   }

   r256Mul(&tq, &tq, b);
   r256Sub(dst, a, &tq);
}

void r256Rsqrt(R256 *dst, const R256 *v)
{
   static const R256 threeHalves = { (R256_U128)1 << 127, 1 };
   R256 x, est;
   int i;

   if ((R256_S128)v->hi < 0) {
      r256Copy(dst, &R256_min);
      return;
   }

   R256_SET2(&x, v->lo, v->hi);

   // get initial estimate
   if (x.hi) {
      int shift = (128 + r256__clz128(x.hi)) >> 1;
      est.lo = (R256_U128)1 << shift;
      est.hi = 0;
   } else if (x.lo) {
      int shift = r256__clz128(x.lo) >> 1;
      est.hi = (R256_U128)1 << shift;
      est.lo = 0;
   } else {
      R256_SET2(dst, 0, 0);
      return;
   }

   // x /= 2
   r256Shr(&x, &x, 1);

   // Newton-Raphson iterate
   for (i = 0; i < 7; ++i) {
      R256 newEst;

      // newEst = est * (threeHalves - (x / 2) * est * est);
      r256__umul(&newEst, &est, &est);
      r256__umul(&newEst, &newEst, &x);
      r256Sub(&newEst, &threeHalves, &newEst);
      r256__umul(&newEst, &est, &newEst);

      if (newEst.lo == est.lo && newEst.hi == est.hi) {
         break;
      }
      R256_SET2(&est, newEst.lo, newEst.hi);
   }

   r256Copy(dst, &est);
}

void r256Sqrt(R256 *dst, const R256 *v)
{
   R256 x, est;
   int i;

   if ((R256_S128)v->hi < 0) {
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
   for (i = 0; i < 7; ++i) {
      R256 newEst;

      // newEst = (est + x / est) / 2
      r256__udiv(&newEst, &x, &est);
      r256Add(&newEst, &newEst, &est);
      r256Shr(&newEst, &newEst, 1);

      if (newEst.lo == est.lo && newEst.hi == est.hi) {
         break;
      }
      R256_SET2(&est, newEst.lo, newEst.hi);
   }

   r256Copy(dst, &est);
}

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

int r256IsNeg(const R256 *v)
{
   R256_ASSERT(v != NULL);

   return (R256_S128)v->hi < 0;
}

void r256Min(R256 *dst, const R256 *a, const R256 *b)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(a != NULL);
   R256_ASSERT(b != NULL);

   if (r256Cmp(a, b) < 0) {
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

   if (r256Cmp(a, b) > 0) {
      r256Copy(dst, a);
   } else {
      r256Copy(dst, b);
   }
}

void r256Floor(R256 *dst, const R256 *v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);

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

void r256Round(R256* dst, const R256* v)
{
   R256_ASSERT(dst != NULL);
   R256_ASSERT(v != NULL);

   dst->hi = v->hi + (v->lo >= ((R256_U128)1 << 127) + (R256_U128)((R256_S128)v->hi < 0));
   dst->lo = 0;
   R256_DEBUG_SET(dst);
}

#endif   //R256_IMPLEMENTATION
