# include<stdio.h>

#ifndef SHOW_BYTES
#define SHOW_BYTES

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len);

void show_int(int x);

void show_float(float x);

void show_pointer(void *x);

void show_short(short x);

void show_long(long x);

void show_double(double x);

#endif