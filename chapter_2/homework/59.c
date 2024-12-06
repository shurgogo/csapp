/*
Write a C expression that will yield a word consisting of the least  significant 
byte of and the remaining bytes of . For operands  and this would give .
*/

unsigned transfer(unsigned x, unsigned y) {
    return x & 0xff | y & ~0U << 8;
}