#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdlib.h>

typedef struct bitset {
	size_t size;
	size_t relative_size;
	unsigned char* set;
} Bitset;

free_bitset(Bitset* set);

#endif // !BITSET_H

