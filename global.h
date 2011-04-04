#ifndef TYPES_H_CMXTN9DF
#define TYPES_H_CMXTN9DF

#include <stdio.h>

typedef unsigned char byte;
typedef unsigned short ushort;

#define DEBUG 1
#define debug_print(fmt, ...) \
	do { if(DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

#endif /* end of include guard: TYPES_H_CMXTN9DF */
