#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 512

int main (int argc, char* argv[]){
	// argc check
	if(argc != 2) {
		perror("Invalid Argument");
		exit(-1);
	}

	// Create a Shared Memory File
	int shmfd = shm_open(argv[1], O_RDWR | O_CREAT, 0666);
	if (shmfd == -1) {
		perror("Shared Memory Open Error: ");
		exit(1);
	}
	// Set the size of the shared memory file to 512
	if(ftruncate(shmfd, SHM_SIZE) == -1) {
		perror("Shared Memory Truncate Error: ");
		exit(1);
	}

	printf("Welcome to My Remote Shell Server\n");

	// Map the shared memory to the current address space
	void *ptr = mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shmfd, 0);
	char *shm = (char*)ptr;
	// Initialized the shard memory by ""
	memset(shm, 0, SHM_SIZE);
	while(1){
		// Reapeat to call usleep(10000) until shm[0] != 0
			printf("Waiting for a command ....\n");
		while(shm[0] == 0) {
			usleep(10000);
		}
		// Copy the command in shm nto a local string variable
		char cmd[512];
		strcpy(cmd, shm);
		// Reset the shared memory to an empty string
		memset(shm, 0, SHM_SIZE);
		// if the command is "exit_svr", break
		if(strcmp(cmd, "exit_svr") == 0)
			break;
		// Cut the command into an argument vector and execute it
		char *ptr = cmd;
		ptr = strtok(ptr, " ");
		char *argument[64] = {NULL,};
		argument[0]= ptr;
		int count = 0;
		while (ptr != NULL) {
			count++;
			ptr = strtok(NULL, " ");
			argument[count] = ptr;
		}
		argument[count] = '\0';
		/*
		for(int i=0; i<=count; i++){
			printf("argument[%d]: %s\n", i, argument[i]);
		}
		*/
		if(strcmp(argument[0], "cd") == 0){
			int result = chdir(argument[1]);
			if (result != 0) {
				perror("cd command error: ");
				exit(1);
			}
		}
		else {
			pid_t child_pid = fork();
			if (child_pid < 0) { // fork failed
				perror("Fork failed: ");
				exit(1);
			}
			else if(child_pid == 0) { // child
				if(execvp(argument[0], argument) == -1) {
					perror("Child Process Execution Error: ");
				}
				exit(0);
			} 
			else { // parent
				wait(NULL);
				continue;
			}
		}

	}
	// Remove the shared memory file
	shm_unlink(ptr);

	// Display a goob-bye message
	printf("Bye!\n");
	return 0;
}
