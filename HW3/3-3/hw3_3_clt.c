#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF 512

int main (int argc, char* argv[]){
	// argc check
	if(argc != 2) {
		perror("Invalid Argument");
		exit(-1);
	}
	printf("Welcome to my remote shell client\n");
	// Open a Shared Memory File
	int shmfd = shm_open(argv[1], O_RDWR, 0666);
	if (shmfd == -1) {
		perror("Shared Memory Open Error: ");
		exit(1);
	}
	// Map the shared memory to the current address space
	void *ptr = mmap(0, BUF, PROT_WRITE, MAP_SHARED, shmfd, 0);
	char* shm = (char*) ptr;
	printf("Connected to server via shared memory\n");
	while(1){
		// Repeat a command as a text line (use fgets())
		char cmd[100];
		fgets(cmd, 100, stdin);
		cmd[strlen(cmd)-1] = '\0';
		// If the command is not an empty string
		if(cmd){
			// Repeat to call usleep(100000) until shm[0] == 0
			while(shm[0] != 0) {
				usleep(10000);
			}
			// If the command is "exit", break
			if(strcmp(cmd, "exit") == 0) {
				break;
			}
			// Copy the command to the shared memory block
			strcpy(shm, cmd);
			// if the command is "exit_svr", break
			if(strcmp(cmd, "exit_svr") == 0)
			break;
		}
	}
	// Display a goob-bye message
	printf("Bye!\n");
}
