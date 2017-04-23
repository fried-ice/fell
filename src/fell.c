#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "vec.h"

#define CMD_EXIT "exit"

/* msg_buff
Describes an pre definied int,
used to determine further acting:
	1 -> exit command
msg_buff */


// Returns null terminated substring
// Takes Char*, start, and end indices
char* getSubstring(char* input, long unsigned start, long unsigned int end) {
	long unsigned int len = end - start;
	char* sub_string = malloc((len) * sizeof(char));
	memcpy(sub_string, &input[start], len * sizeof(char));
	//printf("START %d, END %d, SUBSTRING %s\n", start, end, sub_string);
	return sub_string;
}

unsigned int getArgs(char* input, long unsigned int inp_len, vec* args) {

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
	//printf("NUMBER OF SUBSTRINGS: %d\n", args->count);
}

int handleShellCommands(vec* args, int* msg_buff) {

	if (strncmp(vec_get(args, 0), CMD_EXIT, sizeof(char) * strlen(vec_get(args, 0))) == 0) {
		*msg_buff = 1;
	}
	return 0;
}

int handleChild(vec* args) {
	// Create array
	char** arg_array = (char**)vec_to_array(args);
	/*for (size_t i = 0; i < args->count; i++) {
		printf("IDX %d ARG %s\n", i, (arg_array[i]));
	}*/
	execvp((arg_array[0]), arg_array);
	return 0;
}

int mainLoop() {

	while (1) {
		// Get input from STDIN
		char* inp_buff = 0;
		size_t inp_siz = 0;
		ssize_t inp_len = 0;
		inp_len =  getline(&inp_buff, &inp_len, stdin);
		if (inp_len < 0) {
			perror("Error during read");
			continue;
		}

		// Separate input into substrings and store in vector data structure
		vec args; vec_init(&args);
		getArgs(inp_buff, inp_len, &args);
		//vec_print(args);

		if (args.count == 0) {
			continue;
		}

		// Detect shell specific commands
		int msg_buff;
		handleShellCommands(&args, &msg_buff);
		// Exit shell
		if (msg_buff == 1) {
			printf("Exiting!\n");
			return 0;
		}

		printf("\n");

		// Actual process initialization
		pid_t pid = fork();
		int child_status = 0;

		if (pid < 0) {
			perror("Error during fork");
		} else if (pid == 0) { // Child process
			handleChild(&args);
		} else {
			waitpid(pid, &child_status, WUNTRACED | WCONTINUED);
		}

		free(inp_buff);
		inp_buff = NULL;
		vec_free(&args);
	}
	return 0;
}


int main() {
	printf("Welcome to fell, the furious shell!\n\n");

	return mainLoop();
}
