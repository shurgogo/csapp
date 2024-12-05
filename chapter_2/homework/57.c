/*
Write procedures , and that  print the byte representations of C objects of 
types  and respectively. Try these out on several machines.
*/

#include "show_bytes.h"

void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}
