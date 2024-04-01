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
	time = localtime(&filest.st_mtime);
	sprintf(timestr, "%d/%d/%d %d:%d:%d\n", 1900+time->tm_year, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);

// print the copied file
	sprintf(buf, "attibuts of file \"%s\"\n \tst_dev = %ld\n \tst_mode = %d\n \tst_uid = %d\n \tst_gid = %d\n \tst_mtime = %ld\n \tmodified time = %s", srcFilename, (long)filest.st_dev, filest.st_mode, filest.st_uid, filest.st_gid, (long)filest.st_mtime, timestr);

	write(dest, buf, BUF);
	printf("File attrubutes were successfully copied.\n");

// change mode, uid, guid, atime, mtime of destination file
	struct utimbuf timebuf;
	timebuf.actime = filest.st_atime;
	timebuf.modtime = filest.st_mtime;
	utime(destFilename, &timebuf);

	chmod(destFilename, filest.st_mode);
	chown(destFilename, filest.st_uid, filest.st_gid);

	close(dest);

	return 0;
}
