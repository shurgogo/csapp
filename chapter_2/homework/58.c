/*
Write a procedure that will return 1 when compiled and run on a little-endian 
machine, and will return 0  when compiled and run on a big-endian machine. 
This program  should run on any machine, regardless of its word size.
*/
int is_little_endian() {
    int x = 1;
    return *(char*)&x == 1;
}