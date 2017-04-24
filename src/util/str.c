#include "str.h"

char* getSubstring(char* input, long unsigned start, long unsigned int end) {
	long unsigned int len = end - start;

	char* sub_string = malloc((len) * sizeof(char));
	memcpy(sub_string, &input[start], len * sizeof(char));

	#if VLEVEL > 2
	printf("START %d, END %d, SUBSTRING %s\n", start, end, sub_string);
	#endif

	return sub_string;
}

void getArgs(char* input, long unsigned int inp_len, vec* args) {

	bool in_arg = false;
	long unsigned int start_pos = 0;

	// Separate into strings
	for (long unsigned int i = 0; i < inp_len; ++i) {
		// A new substring starts here
		if (input[i] > 32 && !in_arg) {
			in_arg = true;
			start_pos = i;
		// Leaving current substring
		} else if (input[i] <= 32 && in_arg) {
			in_arg = false;
			vec_add(args, getSubstring(input, start_pos, i));
		}
	}

	#if VLEVEL > 2
	printf("NUMBER OF SUBSTRINGS: %d\n", args->count);
	#endif
}
