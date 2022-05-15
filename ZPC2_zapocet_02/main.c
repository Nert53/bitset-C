#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "bitset.h"

typedef struct bitset {
	size_t size;
	size_t relative_size;
	unsigned char *set;
} Bitset;

Bitset* create_bitset(size_t size) {
	Bitset* new_set = (Bitset*)malloc(sizeof(Bitset));
	if (!new_set) {
		free(new_set);
		return NULL;
	}

	size_t edited_size = ceil((double)size / 8);

	new_set->set = (char*)malloc(edited_size * sizeof(char));
	if (!new_set->set) {
		free(new_set->set);
		free(new_set);
		return NULL;
	}

	for (int i = 0; i < edited_size; i++) {
		new_set->set[i] = 0;
	}

	new_set->size = size;
	new_set->relative_size = edited_size;
	return new_set;
}

Bitset* create_bitset_with_values(size_t size, const int* values, size_t array_size) {
	Bitset* new_set = create_bitset(size);
	if (!new_set) {
		free(new_set);
		return NULL;
	}

	int end_pos = 0;
	for (int j = 0; j < new_set->relative_size; j++) {
		int start_element = (j * 8);
		if (start_element != 0) {
			start_element--;
		}

		for (int i = end_pos; i < array_size; i++) {
			int mask = 1 << (values[i] - (8 * j));
			new_set->set[j] = (new_set->set[j] & ~mask) | (1 << (values[i] - (8 * j)));

			if (values[i + 1] > (8 * (j + 1) - 1)) {
				end_pos = i + 1;
				break;
			}
		}
	}
	
	return new_set;
}

Bitset* create_bitset_with_range(size_t size, int upto) {
	Bitset* new_set = create_bitset(size);
	if (!new_set) {
		free(new_set);
		return NULL;
	}

	int end_pos = 0;
	int sequence = 0;

	for (int j = 0; j < new_set->relative_size; j++) {
		int start_element = (j * 8);
		if (start_element != 0) {
			start_element--;
		}

		for (int i = end_pos; i < 8; i++) {
			int mask = 1 << (sequence - (8 * j));
			new_set->set[j] = (new_set->set[j] & ~mask) | (1 << (sequence - (8 * j)));

			if (sequence + 1 >= upto) {
				end_pos = i + 1;
				return new_set;
			}
			sequence++;
		}
	}

	return new_set;
}

void set_element_change(Bitset* bitset, int element, int binary_value) {
	int pos_in_array = element / 8;
	int relative_pos = element % 8;

	int mask = 1 << (relative_pos);
	bitset->set[pos_in_array] = (bitset->set[pos_in_array] & ~mask) | (binary_value << relative_pos);
}

void set_insert(Bitset* bitset, int element) {
	set_element_change(bitset, element, 1);
}

void set_remove(Bitset* bitset, int element) {
	set_element_change(bitset, element, 0);
}

int contains(Bitset* bitset, int element) {
	int pos_in_array = element / 8;
	int relative_pos = element % 8;

	if (((bitset->set[pos_in_array] >> relative_pos) & 1U) == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int set_expand(Bitset* set, size_t new_size, int start) {
	set->set = (char*)realloc(set->set, new_size);
	if (!set->set) {
		free(set->set);
		return 1;
	}

	for (int i = start; i < set->relative_size; i++) {
		set->set[i] = 0;
	}
	return 0;
}

void form_intersection(Bitset* left, Bitset* right) {
	if (left->size < right->size) {
		int start = right->relative_size - left->relative_size;

		left->size = right->size;
		left->relative_size = right->relative_size;

		int expand = set_expand(left, left->relative_size, start);
		if (expand == 1) {
			return;
		}
	}

	if (left->relative_size > right->relative_size) {
		for (int i = 0; i < right->relative_size; i++) {
			left->set[i] = left->set[i] & right->set[i];
		}
		for (int j = right->relative_size; j < left->relative_size; j++) {
			left->set[j] = 0;
		}
	}
	else {
		for (int i = 0; i < left->relative_size; i++) {
			left->set[i] = left->set[i] & right->set[i];
		}
	}
}

int main() {
	int A[] = { 1, 3, 5 };
	int B[] = { 2, 3, 5, 10, 11, 15 };				// 3, 5
	Bitset* set_A = create_bitset_with_values(6, A, 3);
	Bitset* set_B = create_bitset_with_values(16, B, 6);

	form_intersection(set_B, set_A);

	return 0;
}