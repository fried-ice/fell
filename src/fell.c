#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include "vec.h"
#include "str.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

#define CMD_EXIT "exit"

/* msg_buff
Describes an pre definied int,
used to determine further acting:
	-1 -> empty input
	0  -> regular program execution
	1  -> exit command
msg_buff */

int handleShellCommands(vec* args, int* msg_buff) {

	if (strncmp(vec_get(args, 0), CMD_EXIT, sizeof(char) * strlen(vec_get(args, 0))) == 0) {
		*msg_buff = 1;
	}
	return 0;
}

int handleChild(vec* args) {
	// Create array
	char** arg_array = (char**)vec_to_array(args);
	#if VLEVEL > 2
	for (size_t i = 0; i < args->count; i++) {
		printf("IDX %d ARG %s\n", i, (arg_array[i]));
	}
	#endif
	if (execvp((arg_array[0]), arg_array) < 0) {
			perror("Error on execute");
		}
		_exit(1);
	return 0;
}

int mainLoop() {

	int loop = 1;
	while (loop) {
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
		#if VLEVEL > 1
		vec_print(&args);
		#endif

		int msg_buff = 0;

		// Handle empty input
		if (args.count == 0) {
			msg_buff = -1;
		} else {
			// Detect shell specific commands
			handleShellCommands(&args, &msg_buff);
		}

		// Exit shell
		if (msg_buff == 1) {
			printf("Exiting!\n");
			loop = 0;
		}

		// No shell builtin is detected, try exec on input
		if (msg_buff == 0) {
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
		}

		free(inp_buff);
		inp_buff = NULL;
		vec_free(&args);
	}
	return EXIT_SUCCESS;
}


int main(int argc, char **argv) {
	printf("Welcome to fell, the furious shell!\n");

	static struct option long_options[] = {
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};

	int c = 0;
	while((c = getopt_long(argc, argv, "vh", long_options, NULL)) != -1) {
		switch (c) {
			case 'v':
				printf("fell version %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
				return EXIT_SUCCESS;
			case 'h':
				puts("For help, read fell´s man page - 'man 1 fell'");
				return EXIT_SUCCESS;
		}

	}

	return mainLoop();
}
