#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define INP_BLEN 1024
#define CMD_EXIT "exit"

/* msg_buff
Describes an pre definied int,
used to determine further acting:
	1 -> exit command
msg_buff */

// Removes Newline (\n) and Null bytes at the end of string,
// writes new char array and length
// returns new buffer length 
long unsigned int removeTrailing(char* input, long unsigned int inp_len, char** writeback, long unsigned int* wb_len) {
	*wb_len = inp_len;

	for (long unsigned int i = 0; i < inp_len; ++i) {
		if ((input[i]) < 20) {
			*wb_len = i;
			break;
		}
	}
	*writeback = malloc((*wb_len) * sizeof(char));
	memcpy(*writeback, input, *wb_len * sizeof(char));

	return *wb_len;
}

int handleShellCommands(char* input, long unsigned int lenght, int* msg_buff) {

	if (strncmp(input, CMD_EXIT, sizeof(char) * lenght) == 0) {
		*msg_buff = 1;
	}
	return 0;
}

int handleChild(char* input, long unsigned int lenght) {
	//printf("%s\n", input);
	execl(input, input, NULL);
	return 0;
}


int main() {
	printf("Welcome to fell, the furious shell!\n\n");

	char* inp_buff = NULL;
	char* cmd_buff = NULL;
	long unsigned int inp_len;
	long unsigned int cmd_len;
	int read;
	int msg_buff;

	while (1) {
		read = getline(&inp_buff, &inp_len, stdin);
		removeTrailing(inp_buff, inp_len, &cmd_buff, &cmd_len);
		printf("COMMAND_LENGTH: %d\n", cmd_len);
		for (int i = 0; i<cmd_len; i++) {
			printf("COMMAND_%d: %d\n", i+1, (cmd_buff)[i]);
		}


		if (read < 0) {
			perror("Error during read");
			continue;
		}


		handleShellCommands(cmd_buff, cmd_len, &msg_buff);
		// Exit shell
		if (msg_buff == 1) {
			printf("Exiting!\n");
			return 0;
		}

		printf("\n");

		// Actual process initialization
		int pid = fork();

		if (pid < 0) {
			perror("Error during fork");
		} else if (pid == 0) { // Child process
			handleChild(cmd_buff, inp_len);
		} else {
			waitpid(pid);
		}

		memset(inp_buff, 0, inp_len);
	}
	free (inp_buff);
	return 0;
}