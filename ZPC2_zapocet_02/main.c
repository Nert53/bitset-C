#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "bitset.h"

#define CHAR_BITS 8

typedef struct bitset {
	size_t size;
	size_t relative_size;
	unsigned char *set;
} Bitset;

void free_bitset(Bitset* set) {
	for (int i = 0; i < set->size; i++) {
		set->set[i] = NULL;
		free(set->set[i]);
	}
	free(set->size);
	free(set->relative_size);

}

Bitset* create_bitset(size_t size) {
	Bitset* new_set = (Bitset*)malloc(sizeof(Bitset));
	if (!new_set) {
		free_bitset(new_set);
		return NULL;
	}

	size_t edited_size = ceil((double)size / CHAR_BITS);

	new_set->set = (char*)malloc(edited_size * sizeof(char));
	if (!new_set->set) {
		free_bitset(new_set);
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
		free_bitset(new_set);
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
		free_bitset(new_set);
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
	int pos_in_array = element / CHAR_BITS;
	int relative_pos = element % CHAR_BITS;

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
	int pos_in_array = element / CHAR_BITS;
	int relative_pos = element % CHAR_BITS;

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

char bitwise_and(char a, char b) {
	return (a & b);
}

char bitwise_or(char a, char b) {
	return (a | b);
}

char bitwise_substraction(char a, char b) {
	return ((~b & a) & (b ^ a));
}

int bitwise_subset_test(char a, char b) {
	/* 
	 * A is a subset of B
	 * alternatively we can use '(a & b) == a'
	*/
	if (bitwise_or(a, b) == b) {
		return 1;
	}
	else {
		return 0;
	}
}

void form_operation(Bitset* left, Bitset* right, char (*bitwise_operation)(char, char)) {
	for (int i = 0; i < right->relative_size; i++) {
		left->set[i] = bitwise_operation(left->set[i], right->set[i]);
	}
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

	form_operation(left, right, bitwise_and);
}

Bitset* set_operation(Bitset* left, Bitset* right, size_t new_size, size_t new_edited_size, char (*bitwise_operation)(char, char)) {
	int* values = (int*)malloc(new_edited_size * sizeof(int));
	if (!values) {
		free(values);
		return NULL;
	}

	int arr_size = 0;
	for (int i = 0; i < new_edited_size; i++) {
		char temp_left = left->set[i];
		char temp_right = right->set[i];
		for (int j = 0; j < CHAR_BITS; j++) {
			if (bitwise_operation((temp_left & 0x01), (temp_right & 0x01))) {
				values[arr_size] = (i * 8) + j;
				arr_size++;
			}
			temp_left = temp_left >> 1;
			temp_right = temp_right >> 1;
		}
	}

	return create_bitset_with_values(new_size, values, arr_size);
}

Bitset* set_intersection(Bitset* left, Bitset* right) {
	size_t new_size = min(left->size, right->size);
	size_t new_edited_size = ceil((double)new_size / CHAR_BITS);

	return set_operation(left, right, new_size, new_edited_size, bitwise_and);
}

void form_union(Bitset *left, Bitset *right) {
	form_operation(left, right, bitwise_or);
}

Bitset* set_union(Bitset *left, Bitset *right) {
	size_t new_size = max(left->size, right->size);
	size_t new_edited_size = ceil((double)new_size / CHAR_BITS);

	return set_operation(left, right, new_size, new_edited_size, bitwise_or);
}

void form_difference(Bitset* left, Bitset* right) {
	return form_operation(left, right, bitwise_substraction);
}

Bitset* set_difference(Bitset* left, Bitset* right) {
	size_t new_size = left->size;
	size_t new_edited_size = ceil((double)new_size / CHAR_BITS);

	return set_operation(left, right, new_size, new_edited_size, bitwise_substraction);
}

int is_part_empty(Bitset* set, size_t start) {
	for (size_t i = start; i < set->relative_size; i++) {
		if (set->set[i] != 0) {
			return 0;
		}
	}
	return 1;
}

int is_subset(Bitset* left, Bitset* right) {
	size_t i = 0;
	while (i < left->relative_size) {
		if (bitwise_subset_test(left->set[i], right->set[i]) == 0) {
			return 0;
		}
		if ((i + 1) > right->relative_size && left->relative_size > right->relative_size) {
			if (is_part_empty(left, i + 1)) {
				return 1;
			}
			else {
				return 0;
			}
		}
		i++;
	}

	return 1;
}

int main() {
	char res = bitwise_substraction(40, 25);
	int A[] = { 3, 5, 6 };
	int B[] = { 3, 5, 6 };				// B - A : 0, 4						A - B : 8, 10, 12, 13
	Bitset* set_A = create_bitset_with_values(20, A, 3);
	Bitset* set_B = create_bitset_with_values(7, B, 3);

	is_subset(set_A, set_B);

	return 0;
}