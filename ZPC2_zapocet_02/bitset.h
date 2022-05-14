#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct bitset {
	size_t size;
	unsigned char set_a[];
} Bitset;

#endif // !BITSET_H

