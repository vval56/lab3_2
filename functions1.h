#ifndef FUNCTIONS1_H
#define FUNCTIONS1_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int check();
typedef struct BytesOfInt {
    unsigned char byte1 : 8;
    unsigned char byte2 : 8;
    unsigned char byte3 : 8;
    unsigned char byte4 : 8;
} BytesOfInt;

typedef struct Number {
    int number;
    struct BytesOfInt bytes;
} Number;

void task1();

#endif
