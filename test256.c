#define _CRT_SECURE_NO_DEPRECATE 1

//#define R256_ASSERT(x)
//#define R256_STDC_ONLY
#define R256_IMPLEMENTATION
#ifndef NDEBUG
#  define R256_DEBUG_VIS
#endif

#ifdef __MINGW32__
#  define __USE_MINGW_ANSI_STDIO 1 // force standard sprintf precision
#endif

#include "r256.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static int testsRun, testsFailed;

#if defined(_MSC_VER) && !defined(NDEBUG)
#  include <Windows.h>
#  define PRINT_FAILURE(fmt, ...) do { \
   char buf[1024]; \
   sprintf(buf, fmt, __VA_ARGS__); \
   fputs(buf, stderr); \
   OutputDebugStringA(buf); \
} while(0)
#else
#  define PRINT_FAILURE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#endif

#define R256_TEST_EQ(v1, v2) do { \
   ++testsRun; \
   if ((v1).lo != (v2).lo || (v1).hi != (v2).hi) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%016llx%016llx.%016llx%016llx, expected 0x%016llx%016llx.%016llx%016llx\n", \
         __FILE__, __LINE__, (unsigned long long)R256_R3(&(v1)), (unsigned long long)R256_R2(&(v1)), (unsigned long long)R256_R1(&(v1)), (unsigned long long)R256_R0(&(v1)), \
         (unsigned long long)R256_R3(&(v2)), (unsigned long long)R256_R2(&(v2)), (unsigned long long)R256_R1(&(v2)), (unsigned long long)R256_R0(&(v2))); \
      ++testsFailed; \
   }\
} while(0)

#define R256_TEST_EQ2(v, r0, r1) do { \
   ++testsRun; \
   if ((v).lo != (R256_U128)(r0) || (v).hi != (R256_U128)(r1)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%016llx%016llx.%016llx%016llx, expected 0x%016llx%016llx.%016llx%016llx\n", \
         __FILE__, __LINE__, (unsigned long long)R256_R3(&(v)), (unsigned long long)R256_R2(&(v)), (unsigned long long)R256_R1(&(v)), (unsigned long long)R256_R0(&(v)), \
         (unsigned long long)((R256_U128)(r1) >> 64), (unsigned long long)(r1), (unsigned long long)((R256_U128)(r0) >> 64), (unsigned long long)(r0)); \
      ++testsFailed; \
   }\
} while(0)

#define R256_TEST_EQ4(v, r0, r1, r2, r3) do { \
   ++testsRun; \
   if ((v).lo != (((R256_U128)(r1) << 64) | (r0)) || (v).hi != (((R256_U128)(r3) << 64) | (r2))) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%016llx%016llx.%016llx%016llx, expected 0x%016llx%016llx.%016llx%016llx\n", \
         __FILE__, __LINE__, (unsigned long long)R256_R3(&(v)), (unsigned long long)R256_R2(&(v)), (unsigned long long)R256_R1(&(v)), (unsigned long long)R256_R0(&(v)), \
         (unsigned long long)(r3), (unsigned long long)(r2), (unsigned long long)(r1), (unsigned long long)(r0)); \
      ++testsFailed; \
   }\
} while(0)

#define R256_TEST_STRSTREQ(s1, s2) do { \
   ++testsRun; \
   if (strcmp(s1, s2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: \"%s\" != \"%s\"\n", \
         __FILE__, __LINE__, s1, s2); \
      ++testsFailed; \
   } \
} while(0)

#define R256_TEST_STREQ(r, s) do { \
   R256 rs; \
   r256FromString(&rs, (s), NULL); \
   R256_TEST_EQ(r, rs); \
} while(0)

#define R256_TEST_FLEQ(r, f) do { \
   char bufr[128], buff[128]; \
   r256ToStringf(bufr, sizeof(bufr), "%#.39f", &(r));\
   sprintf(buff, "%#.39f", (double)(f)); \
   R256_TEST_STRSTREQ(bufr, buff); \
} while(0)

#define R256_TEST_FLFLEQ(f1, f2) do { \
   char buf1[128], buf2[128]; \
   sprintf(buf1, "%.39f", (double)(f1)); \
   sprintf(buf2, "%.39f", (double)(f2)); \
   R256_TEST_STRSTREQ(buf1, buf2); \
} while(0)

#define R256_TEST_INTINTEQ(i1, i2) do { \
   ++testsRun; \
   if ((int)(i1) != (int)(i2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: %d != %d\n", \
         __FILE__, __LINE__, (int)(i1), (int)(i2)); \
      ++testsFailed; \
   } \
} while(0)

static void test_float()
{
   double a;
   double b;
   R256 c;

   a = -2.125;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = 0.25;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = -0.25;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = -2.5;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = 2.118018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = 2.918018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = 2.518018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = -2.118018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = -2.918018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);

   a = 2.518018798719000910681;
   r256FromFloat(&c, a);
   R256_TEST_FLEQ(c, a);
   b = r256ToFloat(&c);
   R256_TEST_FLFLEQ(b, a);
}

static void test_string()
{
   char bufa[512];
   char bufb[512];
   R256 a, b;
   double d;
   int reta, retb;
   char *endptr;

   d = 0.9999;
   r256FromFloat(&a, d);

   retb = snprintf(bufb, sizeof(bufb), "%5.6f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%5.6f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%1.0f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%1.0f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%# 3.0f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%# 3.0f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%-20.4f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%-20.4f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);
   retb = snprintf(bufb, sizeof(bufb), "%#+.50f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%#+.50f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);

   d = (1.0 / 340282366920938463463374607431768211456.0);
   r256FromFloat(&a, d);

   retb = snprintf(bufb, sizeof(bufb), "%1.200f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%1.200f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);

   retb = snprintf(bufb, sizeof(bufb), "%1.300f", d);
   reta = r256ToStringf(bufa, sizeof(bufa), "%1.300f", &a);
   R256_TEST_STRSTREQ(bufa, bufb);
   R256_TEST_INTINTEQ(reta, retb);

   r256FromInt(&b, 1);
   r256FromString(&a, "1", &endptr);
   R256_TEST_EQ(a, b);
   R256_TEST_INTINTEQ(*endptr, '\0');
   r256FromString(&a, "+1.", &endptr);
   R256_TEST_INTINTEQ(*endptr, '\0');
   R256_TEST_EQ(a, b);
   r256FromString(&a, "1.0", &endptr);
   R256_TEST_INTINTEQ(*endptr, '\0');
   R256_TEST_EQ(a, b);
   r256FromString(&a, "1.0 xxxxxxx", &endptr);
   R256_TEST_INTINTEQ(*endptr, ' ');
   R256_TEST_EQ(a, b);
}

static void test_sign()
{
    R256 a, b, c;
    r256FromFloat(&a, -4.25);
    r256FromFloat(&b, 4.25);

    R256_TEST_INTINTEQ(r256IsNeg(&a), 1);
    R256_TEST_INTINTEQ(r256IsNeg(&b), 0);

    r256Neg(&c, &a);
    R256_TEST_EQ(c, b);

    r256Neg(&c, &b);
    R256_TEST_EQ(c, a);

    r256Abs(&c, &a);
    R256_TEST_EQ(c, b);

    r256Abs(&c, &b);
    R256_TEST_EQ(c, b);

    r256Nabs(&c, &a);
    R256_TEST_EQ(c, a);

    r256Nabs(&c, &b);
    R256_TEST_EQ(c, a);
}

static void test_cmp()
{
   R256 a, b, c, d;
   int cmp;

   // Create test values for 256-bit comparison
   // a = 1.5 (hi=1, lo=0x80000000000000000000000000000000)
   R256_SET2(&a, (R256_U128)1 << 127, 1);
   
   // b = 1.25 (hi=1, lo=0x40000000000000000000000000000000)
   R256_SET2(&b, (R256_U128)1 << 126, 1);
   
   // c = -0.5 (hi=0, lo=0x80000000000000000000000000000000 sign-extended)
   R256_SET2(&c, (R256_U128)1 << 127, (R256_U128)-1);
   
   // d = -0.75 (hi=0, lo=0x40000000000000000000000000000000 sign-extended)
   R256_SET2(&d, (R256_U128)1 << 126, (R256_U128)-1);

   cmp = r256Cmp(&a, &a);
   R256_TEST_FLFLEQ(cmp, 0);
   cmp = r256Cmp(&a, &b);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&a, &c);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&a, &d);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&b, &a);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&b, &b);
   R256_TEST_FLFLEQ(cmp, 0);
   cmp = r256Cmp(&b, &c);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&b, &d);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&c, &a);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&c, &b);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&c, &c);
   R256_TEST_FLFLEQ(cmp, 0);
   cmp = r256Cmp(&c, &d);
   R256_TEST_FLFLEQ(cmp, 1);
   cmp = r256Cmp(&d, &a);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&d, &b);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&d, &c);
   R256_TEST_FLFLEQ(cmp, -1);
   cmp = r256Cmp(&d, &d);
   R256_TEST_FLFLEQ(cmp, 0);
}

static void test_div()
{
   R256 a, b, c;

   r256FromString(&a, "97276714306369.00003331527114698671", NULL);
   r256FromString(&b, "23806.00000639050267636776", NULL);
   r256Div(&c, &a, &b);
   R256_TEST_STREQ(c, "4086226761.3314272443409737572619997338651003847204");

   r256FromString(&a, "10", NULL);
   r256FromString(&b, "3", NULL);
   r256Div(&c, &a, &b);
   R256_TEST_STREQ(c, "3.3333333333333333333333333333333333333333333333333");

   r256FromString(&a, "2113123919594", NULL);
   r256FromString(&b, "-11943", NULL);
   r256Div(&c, &a, &b);
   R256_TEST_STREQ(c, "-176934096.92656786402076530185045633425437494766809");

   r256FromString(&a, "62727997390472", NULL);
   r256FromString(&b, "154", NULL);
   r256Div(&c, &a, &b);
   R256_TEST_STREQ(c, "407324658379.68831168831168831168831168831168831169");

   r256FromInt(&a, 100);
   r256FromString(&b, "10.003048780487804878", NULL);
   r256Div(&c, &a, &b);
   R256_TEST_STREQ(c, "9.9969521487351417251325666669856026417562087981322");

   r256Copy(&a, &R256_one);
   r256Shr(&a, &a, 2);
   r256Div(&c, &a, &R256_min);
   R256_TEST_FLEQ(c, 0);

   r256Copy(&b, &R256_one);
   r256Shr(&b, &b, 1);     // b = .5
   r256Shr(&a, &b, 1);     // a = .25
   r256Div(&c, &a, &b);
   R256_TEST_EQ(c, b);

   r256Div(&c, &b, &a);
   R256_TEST_EQ2(c, 0, 2);

   r256Div(&c, &R256_one, &R256_smallest);
   R256_TEST_EQ(c, R256_max);
}

static void test_mod()
{
   R256 a, b, c;

   r256FromFloat(&a, 5.3);
   r256FromFloat(&b, 2);
   r256Mod(&c, &a, &b);
   R256_TEST_FLEQ(c, fmod(5.3, 2));

   r256FromFloat(&a, -18.5);
   r256FromFloat(&b, 4.2);
   r256Mod(&c, &a, &b);
   R256_TEST_FLEQ(c, fmod(-18.5, 4.2));
}

static void test_shift()
{
   R256 a, b;

   R256_SET4(&a, 5, 0, 0, 0);
   r256Shl(&b, &a, 1);
   R256_TEST_EQ4(b, 0x000000000000000a, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000);
   r256Shl(&b, &a, 65);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x000000000000000a, 0x0000000000000000, 0x0000000000000000);
   r256Shl(&b, &a, 129);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x000000000000000a, 0x0000000000000000);
   r256Shl(&b, &a, 193);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x000000000000000a);

   R256_SET4(&a, 5, 0, 0, 0);
   r256Shl(&b, &a, 0);
   R256_TEST_EQ4(b, 0x0000000000000005, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000);
   r256Shl(&b, &a, 64);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000005, 0x0000000000000000, 0x0000000000000000);
   r256Shl(&b, &a, 128);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000005, 0x0000000000000000);
   r256Shl(&b, &a, 192);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000005);

   R256_SET4(&a, 0, 0, 0, 0xa000000000000000);
   r256Shr(&b, &a, 1);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x5000000000000000);
   r256Shr(&b, &a, 65);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x5000000000000000, 0x0000000000000000);
   r256Shr(&b, &a, 129);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x5000000000000000, 0x0000000000000000, 0x0000000000000000);
   r256Shr(&b, &a, 193);
   R256_TEST_EQ4(b, 0x5000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000);

   r256Shr(&b, &a, 0);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xa000000000000000);
   r256Shr(&b, &a, 64);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0xa000000000000000, 0x0000000000000000);
   r256Shr(&b, &a, 128);
   R256_TEST_EQ4(b, 0x0000000000000000, 0xa000000000000000, 0x0000000000000000, 0x0000000000000000);
   r256Shr(&b, &a, 192);
   R256_TEST_EQ4(b, 0xa000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000);

   r256Sar(&b, &a, 1);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xd000000000000000);
   r256Sar(&b, &a, 65);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0xd000000000000000, 0xffffffffffffffff);
   r256Sar(&b, &a, 129);
   R256_TEST_EQ4(b, 0x0000000000000000, 0xd000000000000000, 0xffffffffffffffff, 0xffffffffffffffff);
   r256Sar(&b, &a, 193);
   R256_TEST_EQ4(b, 0xd000000000000000, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);

   r256Sar(&b, &a, 0);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xa000000000000000);
   r256Sar(&b, &a, 64);
   R256_TEST_EQ4(b, 0x0000000000000000, 0x0000000000000000, 0xa000000000000000, 0xffffffffffffffff);
   r256Sar(&b, &a, 128);
   R256_TEST_EQ4(b, 0x0000000000000000, 0xa000000000000000, 0xffffffffffffffff, 0xffffffffffffffff);
   r256Sar(&b, &a, 192);
   R256_TEST_EQ4(b, 0xa000000000000000, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff);
}

static void test_sqrt()
{
   R256 a, b;

   r256FromInt(&a, 100);
   r256Sqrt(&b, &a);
   R256_TEST_STREQ(b, "10");

   r256FromInt(&a, 2);
   r256Sqrt(&b, &a);
   R256_TEST_STREQ(b, "1.41421356237309504880");

   r256FromFloat(&a, 0.125);
   r256Sqrt(&b, &a);
   R256_TEST_STREQ(b, "0.35355339059327376220");

   r256FromFloat(&a, 0.999);
   r256Rsqrt(&b, &a);
   R256_TEST_STREQ(b, "1.00050037531277368426");

   r256FromInt(&a, 267794469);
   r256Sqrt(&b, &a);
   R256_TEST_STREQ(b, "16364.42693772073400801913");
}

static void test_floor()
{
   R256 a, b, c;

   r256FromFloat(&a, -1.75);

   r256Floor(&b, &a);
   r256FromFloat(&c, -2);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 1.75);
   r256Floor(&b, &a);
   r256FromFloat(&c, 1);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, -0.75);
   r256Floor(&b, &a);
   r256FromFloat(&c, -1);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 0.75);
   r256Floor(&b, &a);
   r256FromFloat(&c, 0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 0);
   r256Floor(&b, &a);
   R256_TEST_EQ(b, a);

   r256FromFloat(&a, -2);
   r256Floor(&b, &a);
   R256_TEST_EQ(b, a);

   r256FromFloat(&a, 2);
   r256Floor(&b, &a);
   R256_TEST_EQ(b, a);
}

static void test_ceil()
{
   R256 a, b, c;

   r256FromFloat(&a, -1.75);
   r256Ceil(&b, &a);
   r256FromFloat(&c, -1);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 1.75);
   r256Ceil(&b, &a);
   r256FromFloat(&c, 2);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, -0.75);
   r256Ceil(&b, &a);
   r256FromFloat(&c, 0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 0.75);
   r256Ceil(&b, &a);
   r256FromFloat(&c, 1);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 0);
   r256Ceil(&b, &a);
   R256_TEST_EQ(b, a);

   r256FromFloat(&a, -2);
   r256Ceil(&b, &a);
   R256_TEST_EQ(b, a);

   r256FromFloat(&a, 2);
   r256Ceil(&b, &a);
   R256_TEST_EQ(b, a);
}

static void test_int()
{
   R256 a;

   r256FromFloat(&a, -1.75);
   R256_TEST_INTINTEQ(r256ToInt(&a), -1);

   r256FromFloat(&a, 1.75);
   R256_TEST_INTINTEQ(r256ToInt(&a), 1);

   r256FromFloat(&a, -0.75);
   R256_TEST_INTINTEQ(r256ToInt(&a), 0);

   r256FromFloat(&a, 0.75);
   R256_TEST_INTINTEQ(r256ToInt(&a), 0);

   r256FromFloat(&a, 0);
   R256_TEST_INTINTEQ(r256ToInt(&a), 0);

   r256FromFloat(&a, -2);
   R256_TEST_INTINTEQ(r256ToInt(&a), -2);

   r256FromFloat(&a, 2);
   R256_TEST_INTINTEQ(r256ToInt(&a), 2);
}

static void test_round()
{
   R256 a, b, c;

   r256FromFloat(&a, 0);
   r256Round(&b, &a);
   R256_TEST_EQ(b, a);

   r256FromFloat(&a, 2.3);
   r256Round(&b, &a);
   r256FromFloat(&c, 2.0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 2.5);
   r256Round(&b, &a);
   r256FromFloat(&c, 3.0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, 2.7);
   r256Round(&b, &a);
   r256FromFloat(&c, 3.0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, -2.3);
   r256Round(&b, &a);
   r256FromFloat(&c, -2.0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, -2.5);
   r256Round(&b, &a);
   r256FromFloat(&c, -3.0);
   R256_TEST_EQ(b, c);

   r256FromFloat(&a, -2.7);
   r256Round(&b, &a);
   r256FromFloat(&c, -3.0);
   R256_TEST_EQ(b, c);
}

int main()
{
   test_float();
   test_string();
   test_sign();
   test_cmp();
   test_mod();
   test_div();
   test_shift();
   test_sqrt();
   test_floor();
   test_ceil();
   test_int();
   test_round();

   printf("%d tests run. %d tests passed. %d tests failed.\n",
      testsRun, testsRun - testsFailed, testsFailed);

   return testsFailed;
}
