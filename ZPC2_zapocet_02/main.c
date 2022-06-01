#include "bitset.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	//testing fucntions

	int A[] = { 1, 2, 3, 4, 5, 6 };
	int B[] = { 2, 4, 6, 8, 10, 12, 14, 15 };

	Bitset* set_A = create_bitset_with_values(7, A, 6);
	Bitset* set_B = create_bitset_with_values(13, B, 8);
	Bitset* set_C = create_bitset_with_range(40, 25);

	Bitset** array = load_bitsets("saved_sets.txt");
}