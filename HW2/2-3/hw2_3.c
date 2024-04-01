#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <utime.h>

#define BUF 512

int main(int argc, char *argv[])
{
	int src, dest;
	char srcFilename[32], destFilename[32];
	struct stat filest;
	char buf[BUF]="";

	printf("%s %s\n", argv[1], argv[2]);

	if(argc != 3) {
		printf("Wrong argument");
		exit(EXIT_FAILURE);
	}
	strcpy(srcFilename, argv[1]);
	strcpy(destFilename, argv[2]);

	if(stat(srcFilename, &filest) == -1) {
		perror("Stat error: ");
		exit(EXIT_FAILURE);
	}
	
// open dest file
	dest = open(destFilename, O_WRONLY);
	if(dest == -1){
		perror("Destination File Open Error: ");
		exit(EXIT_FAILURE);	
	}
	
// time display
	char timestr[50]="";
	struct tm *time;
	time = localtime(filest.st_mtime);
	sprintf(timestr, "%d/%d/%d %d:%d:%d\n", 1900+time->tm_year, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);

// print the copied file
	strcat(buf, "attributes of file");
	strcat(buf, "\"");
	strcat(buf, srcFilename);
	strcat(buf, "\"\n");
	strcat(buf, "st_dev = ");
	strcat(buf, filest.st_dev);
	strcat(buf, "\nst_mode = ");
	strcat(buf, filest.st_mode);
	strcat(buf, "\nst_uid = ");
	strcat(buf, filest.st_uid);
	strcat(buf, "\nst_gid = ");
	strcat(buf, filest.st_gid);
	strcat(buf, "\nst_mtime = ");
	strcat(buf, filest.st_mtime);
	strcat(buf, "\nmodified time = ");
	strcat(buf, timestr);

	write(dest, buf, BUF);
	printf("File attrubutes were successfully copied.\n");

// change mode, uid, guid, atime, mtime of destination file
	struct utimbuf timebuf;
	timebuf.actime = filest.st_atime;
	timebuf.modtime = filest.st_mtime;
	utime(destFilename, &timebuf);

	chmod(destFilename, filest.st_mode);
	chown(destFilename, filest.st_uid, filest.st_gid);

	close(src);
	close(dest);

	return 0;
}
