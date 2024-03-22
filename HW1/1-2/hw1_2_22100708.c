#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF 256

int main()
{
	char input[BUF], output[BUF*2];
	char *cur;

	printf("input a text: \n");
	fgets(input, BUF, stdin);
	input[strlen(input)-1]='\0';
	
	cur = &input[0];
	strcpy(output,"");

	for (int i=0; i<strlen(input); i++)
	{
		if (!isspace(*cur)) 
		{
			if (i == 0 || isspace(*(cur-1))) {
				strcat(output, "[");
			}
			else {
				strcat(output, " ");
			}
		}
		else 
		{
			if (!isspace(*(cur-1)) || cur==NULL) { // the end of word
				strcat(output, "]");
			}
			else {
				strcat(output, " ");
			}
		}
		cur++;
	}
	if (!isspace(input[strlen(input)-2])) 
		strcat(output,"]");

	printf("%s\n", output);

	return 0;

}
