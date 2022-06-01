#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdlib.h>

#define edit_size(size) (((size) + ((size) % CHAR_BIT)) / CHAR_BIT)

typedef struct bitset {
	size_t size;
	size_t relative_size;
	unsigned char* set;
} Bitset;

typedef struct help_nums {
	int* numbers;
	int pos;
} Help_nums;

Bitset* create_bitset(size_t size);
Bitset* create_bitset_with_values(size_t size, const int* values, size_t array_size);
Bitset* create_bitset_with_range(size_t size, int upto);

void set_insert(Bitset* bitset, int element);
void set_remove(Bitset* bitset, int element);
int contains(Bitset* bitset, int element);

void form_intersection(Bitset* left, Bitset* right);
Bitset* set_intersection(Bitset* left, Bitset* right);

void form_union(Bitset* left, Bitset* right);
Bitset* set_union(Bitset* left, Bitset* right);

void form_difference(Bitset* left, Bitset* right);
Bitset* set_difference(Bitset* left, Bitset* right);

int is_subset(Bitset* left, Bitset* right);

int save_bitsets_to_file(char* file, Bitset** bitsets, size_t bitsets_count);
Bitset** load_bitsets(char* file);

#endif // !BITSET_H

