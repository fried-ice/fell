#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>

#include "vec.h"
#include "str.h"

#define VERSION_MAJOR "0"
#define VERSION_MINOR "2"
#define VERSION_MICRO "0"

#define CMD_EXIT "exit"
#define CMD_CD "cd"

/* msg_buff
Describes an pre definied int,
used to determine further acting:
	-1 -> empty input
	0  -> regular program execution
	1  -> exit command
	2  -> change directory command
msg_buff */

int handleShellCommands(vec* args, int* msg_buff) {

	if (strncmp(vec_get(args, 0), CMD_EXIT, sizeof(char) * strlen(vec_get(args, 0))) == 0) {
		*msg_buff = 1;
	} else if (strncmp(vec_get(args, 0), CMD_CD, sizeof(char) * strlen(vec_get(args, 0))) == 0) {
		*msg_buff = 2;
	}
	return 0;
}

int handleChild(vec* args) {
	// Create array
	char** arg_array = (char**)vec_to_array(args);
	#if VLEVEL > 2
	puts("EXEC ARGS:");
	for (size_t i = 0; i < args->count; i++) {
		printf("Index: %d, Argument \"%s\"\n", i, (arg_array[i]));
	}
	puts("");
	#endif
	if (execvp((arg_array[0]), arg_array) < 0) {
			perror("Error on execute");
	}
	_exit(1);
	return 0;
}

// This function is rather an experiment - but hey, it seems to work
char* getCurrentWorkingDir() {
	// Add 32 characters to the buffer until its large enough
	// for content to fit
	#define CWD_BUFF_SIZE 32
	size_t buff_size = CWD_BUFF_SIZE * sizeof(char);
	char* buff = malloc(buff_size);

	// Increase buffer if getcwd() fails with errno 34 - ENOMEM (Not enough space)
	while (getcwd(buff, buff_size) == NULL && errno == 34) {
		#if VLEVEL > 1
		printf("CWD Buffer too small (%zu), increasing by %d\n", buff_size, CWD_BUFF_SIZE);
		#endif
		buff_size += CWD_BUFF_SIZE * sizeof(char);
		buff = realloc(buff, buff_size);
	}
	return buff;
}

int mainLoop() {

	int loop = 1;
	while (loop) {

		// Display current working directory
		char* cwd = getCurrentWorkingDir();
		printf("%s >> ", cwd);
		free(cwd);


		// Get input from STDIN
		char* inp_buff = 0;
		size_t inp_len = 0;
		inp_len =  getline(&inp_buff, &inp_len, stdin);
		#if VLEVEL > 2
		printf("GETLINE: \"%s\"\n\n", inp_buff);
		#endif
		if (inp_len == 0) {
			perror("Error during read");
			free(inp_buff);
			inp_buff = NULL;
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

		if (msg_buff == 2) {
			if (chdir(vec_get(&args, 1)) < 0) {
				perror("Directory not existent");
			}
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
				printf("fell version %s.%s.%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
				return EXIT_SUCCESS;
			case 'h':
				puts("For help, read fell´s man page - 'man 1 fell'");
				return EXIT_SUCCESS;
		}

	}

	return mainLoop();
}
