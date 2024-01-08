/* 
 * CS:APP Data Lab 
 * 
 * <Lee Jaewon 20210489>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this 
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.
&&, ||, -, or 
 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
//1
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return ~x&~y;
}
/* 
 * fitsShort - return 1 if x can be represented as a 
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
    int a = x >> 15;
    return ((!a)) | (!(~a));
}
/*
 * isMinOrMaxofT - returns 1 if x is the minimum or the maximum of
 *      two's complement number, and returns 0 otherwise 
 *    
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 20
 *   Rating: 1
 */
int isMinOrMaxofT(int x) { 
  int a = ~x;
  return ((!(((a + 1) ^ x) & ((x + 1) ^ a))) & (!((!x) | (!a))));
} 
//2   
/*
 * AverageofTwo - calculate floor(average of two integers). 
 *   Floor function takes as input a real number,
 *   and gives out the greatest integer less than or equal to x.
 *   Examples: AverageofTwo(1, 2) = 1
 *             AverageofTwo(30, 40) = 35    
 *             AverageofTwo(-1, -2) = -2    
 *             AverageofTwo(-30, -40) = -35     
 *             AverageofTwo(-2147483648, -2147483648) = -2147483648    
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 2 
 *   끝에가 둘 다 홀수인 경우에는 문제가 생김 -> 확인해주기 &이용
 */
int AverageofTwo(int x, int y) {
    int a = x>>1;
    int b = y>>1;
  return  a+b+(x&y&0x1);
}
/*
 * isAlphabetCharacter - returns whether x is an ascii alphabet 'A'~'z'
 *   Example: isAlphabetCharacter('{') = 0, isAlphabetCharacter('a') = 1,
 *            isAlphabetCharacter('Z') = 1, isAlphabetCharacter('&') = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 2
 */
int isAlphabetCharacter(int x) {
    int a = (!((x>>5)^0x2) | !((x>>5)^0x3));
    int b = !( !((x&0x1f)) | !((x&0x1f)^0x1B) | !((0x7&(x>>2))^0x7));
    return a&b;
}
/* 
 * changeByte - change byte n from word x to m
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   You can assume 0 <= n <= 3 and 0 <= m <= 255
 *   Examples: changeByte(0x12 34 56 78,1, 0x12) = 0x12341278
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int changeByte(int x, int n, int m) {
  int new_x = x&(~(0xFF<<(n<<3)));
  int new_m = m<<(n<<3);
  return new_x | new_m;
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    int a = 0x80000000 ^ uf;  //부호 
    int b = 0x7F800000 & uf;  
    int c = 0x007FFFFF & uf;

    if((b == 0x7F800000) && c) 
        return uf;
    return a;

}
//3
/* 
 * float_quarter - Return bit-level equivalent of expression 0.25*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
unsigned float_quarter(unsigned uf) {
  unsigned exp = (uf & 0x7f800000) >> 23;
  unsigned frac = uf & 0x7fffff;
  unsigned sign = uf & 0x80000000;

  if (exp == 0xff){ // if NaN
    return uf;
  }
  if( exp <= 3 ){
    if ((uf & 0x7) == 6){
      uf += 2;
    }
    if ((uf & 0x3) == 0x3){
      uf += 1;
    }
    int uff = uf^sign;
    uf = sign | (uff >> 2);
    return uf;
  }
  return sign | ((exp - 2) << 23) | frac;
}
/* 
 * LSBFillLeftShift - shift x to the left by n, while filling bit at LSB
 *   Can assume that 0 <= n <= 31
 *   Examples: LSBFillLeftShift(0x87654321,8) = 0x654321ff
 *             LSBFillLeftShift(0x87654300,8) = 0x65430000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 *  1. x를 left로 옮기기
 *  2. LSB n bit짜리로 만들기
 *  3. x + LSB n bit
 */
  int LSBFillLeftShift(int x, int n) {
    int new_x = (x<<n);
    int L = (1<<n) + (~0) ;
    int check = ~(((x&0x1))+ (~0));
    return ((check)&(new_x+L)) | ((~check)&(new_x));
  }


/*
 * hexAlphabetCount - returns count of number of alphabet in hexadecimal
 *   Examples: hexAlphabetCount(0xABCDEF00) = 6
 *             hexAlphabetCount(0x12345600) = 0
 *             hexAlphabetCount(0xABC45600) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 3
 */ 
int hexAlphabetCount(int x) {
    int ans = (x >> 3) & (((x >> 2) | (x >> 1))) & 0x1;
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    ans = ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
    x = (x >> 4);
    return ans + ((x >> 3) & (((x >> 2) | (x >> 1))) & 0x1);
}
/*
 * Mul4 - multiply by 4, saturating to Tmin or Tmax if overflow
 *   Examples: Mul4(0x10000000) = 0x40000000
 *             Mul4(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             Mul4(0xD0000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 3
 */
int Mul4(int x) {
  // if MSB is 0, 2nd and 3rd bit must be 0
  // if MSB is 1, 
  int a = x >> 29;
  int over = ((((!a)) | (!(~a))) + ~0);
  return ((~over) & (x << 2)) | (over & (((0x7F << 24) | (0xFF << 16) | (0xFF << 8) | 0xFF) + (!((x >> 31) + 1))));
  return 2;
}
/* 
 * isSubOF - Determine if can compute x-y without overflow
 *   Example: isSubOF(0x80000000,0x80000000) = 1,
 *            isSubOF(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >> 
 *   Max ops: 20
 *   Rating: 3
 */
int isSubOF(int x, int y) {
  int a = (x >> 31);
  int b = (y >> 31);  
  int sign = (!a)^(!b);
  int c = ~y + 1; // 2의보수
  int check = (!((x+c)>>31)) ^ (!a);
  return !(sign&check);
}
/* 
 * countOnesInPart - returns the number of 1's in particular regions in word.
 *   Example: countOnesInPart(0x1, 3, 0) = 1, countOnesInPart(0x2, 3, 0) = 1,
 *            countOnesInPart(0x3, 3, 0) = 2, countOnesInPart(0x7, 3, 0) = 3,
 *            countOnesInPart(0x10, 3, 0) = 0, countOnesInPart(0x12345678, 20, 8) = 6
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then returns 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 3
 */
int countOnesInPart(int x, int highbit, int lowbit) {
    x = x << (32 + ~highbit);
    x = x & ((~(1 << (32 + ~highbit + lowbit))) + 1);
    int count = 0x11 | (0x11 << 8); 
    int sum = 0;
    int mask = 0xF | (0xF << 8); 

    int lowlow = ((lowbit + ~highbit) >> 31);
    count = count | (count << 16); 
    sum = x & count; 
    sum = sum + ((x >> 1)&count); 
    sum = sum + ((x >> 2)&count); 
    sum = sum + ((x >> 3)&count); 
    sum = sum + (sum >> 16);
    sum = (sum&mask) + ((sum >> 4)&mask);
    return lowlow & ((sum & 0xFF) + (sum >> 8));
}
//4
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {

  // Transfer 1st zero
  int a = x;
  int ffff = ((0xff << 8) | 0xff);
  a = ((a >> 1) & a);
  a = ((a >> 2) & a);
  a = ((a >> 4) & a);
  a = ((a >> 8) & a);
  a = ((a >> 16) & a);
  
  // Binary Search
  a = (~a + 1);
  int b = a;
  int ans = (!(a & ffff)) << 4;
  a = ((a >> 16) & ffff) | (a & ffff);

  ans = ans + ((!(a & 0xff)) << 3);
  a = ((a >> 8) & 0xff) | (a & 0xff);
  
  ans = ans + ((!(a & 0xf)) << 2);
  a = ((a >> 4) & 0xf) | (a & 0xf);

  ans = ans + ((!(a & 0x3)) << 1);
  a = ((a >> 2) & 0x3) | (a & 0x3);

  ans = ans + !(a & 0x1);
  return (32 + ~ans + !!a);
}
/*
 * MulNineoverSixteen - multiply by 9/16 rounding toward 0, avoiding overflow.
 *  Examples: MulNineoverSixteen(22) = 12
 *            MulNineoverSixteen(421) = 236
 *            MulNineoverSixteen(-121) = -68
 *            MulNineoverSixteen(-129) = -72
 *            MulNineoverSixteen(-158733) = -89287
 *            MulNineoverSixteen(0x40000000) = 603979776 (no overflow)
 *            MulNineoverSixteen(0x8F000000) = -1066401792 (no overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 4
 */
int MulNineoverSixteen(int x) {
  int a = (x & 0x1) << 3;
  int b = x & 0xf;
  int c = a+b;
  int over = c & 0x10;
  int down = (c & 0xf) & (x >> 31);

  return (x >> 1) + (x >> 4) + !!over + !!down;
}
/*
 * myLog2 - return floor(log base 2 of x), where x > 0
 *   Example: myLog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int myLog2(int x) {
    int a = 0x11 | (0x11 <<8);
    int sum = 0;
    x= x| (x>>1);
    x= x| (x>>2);
    x= x| (x>>4);
    x= x| (x>>8);
    x= x| (x>>16);

    a = a | (a<<16);
    sum = sum + (x&a);
    sum = sum + ((x>>1)&a);
    sum = sum + ((x>>2)&a);
    sum = sum + ((x>>3)&a);
    sum = sum + (sum >>16);

    int c = 0xF | (0xF <<8);

    sum = (sum&c)+ ((sum>>4)&c);

    return ((sum&0xFF)+(sum>>8))+(~0);

}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int neg = (x >> 31);
  return (neg & (~x + 1)) | (~neg & x);
}
/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
	unsigned a = uf >> 31;
	unsigned b = (uf >> 23) & 0xFF;
	unsigned frac =(uf & 0x7FFFFF);
	unsigned c = (uf & 0x7FFFFF);

  if (b < 0x7F)
    return 0x0;

  if (b == 0xFF) 
    return 0x80000000u;

  b = b - 0x7F;
  
  if (b >= 31)
    return 0x80000000u;

  if (b > 22) 
    c = frac << (b - 23);
  else 
    c = frac >> (23 - b);

  c = c + 1 << b;
  if (a)
    c = -c;

  return c;
}
/* 
 * leastBitPosition - return a position of the least significant 1 bit. 
 *               count from right side of the bit string.
 *               In other words, bits are numbered from 1(LSB) to 32(MSB).
 *               For example, position of least significant bit of 0x20 is 6.
 *               If there are no 1 bit in bit string, it should return 0.
 *               If x == 0, return 0
 *   Example: leastBitPosition(96) = 6, leastBitPosition(31) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int leastBitPosition(int x) {
  // Transfer 1st zero
  int a = x;
  int ffff = (0xff << 8) | 0xff;
  a = ((a << 1) | a);
  a = ((a << 2) | a);
  a = ((a << 4) | a);
  a = ((a << 8) | a);
  a = ((a << 16) | a);

  // Binary Search
  a = (~a + 1);
  int b = a;
  int ans = (!(a & ffff)) << 4;
  a = ((a >> 16) & ffff) | (a & ffff);

  ans = ans + ((!(a & 0xff)) << 3);
  a = ((a >> 8) & 0xff) | (a & 0xff);
  
  ans = ans + ((!(a & 0xf)) << 2);
  a = ((a >> 4) & 0xf) | (a & 0xf);

  ans = ans + ((!(a & 0x3)) << 1);
  a = ((a >> 2) & 0x3) | (a & 0x3);

  ans = ans + !(a & 0x1);

  return ((ans + !b) & 0x1f) + !!b;

}
