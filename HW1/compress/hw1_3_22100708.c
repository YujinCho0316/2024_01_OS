#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF 256
#define BIT 25

struct st_word {
	int start;
	int end;
	char word[BIT];
};

int main()
{
	char input[BUF], output[BUF*2];
	char *cur;
	struct st_word *words[10];
	int count = 0;

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
				words[count] = (struct st_word*)malloc(sizeof(struct st_word));
				words[count]->start = i;
			//	strcpy(words[count]->word, "");
			//	strcat(words[count]->word, input[i]);
			}
			else {
				strcat(output, " ");
			//	strcat(words[count]->word, input[i]);
			}
		//	printf("i:%d, count:%d\n", i, count);
		}
		else 
		{
			if (!isspace(*(cur-1)) || cur==NULL) { // the end of word
				strcat(output, "]");
				words[count]->end = i;
				count++;
			//	printf("i:%d, count:%d\n", i, count);
			//	printf("words[%d] = (%d, %d, %s)\n", count-1, words[count-1]->start, words[count-1]->end, words[count-1]->word);
			}
			else {
				strcat(output, " ");
			}
		}
		cur++;
	}


	char tmp[BUF];
	char *token;
	strcpy(tmp, input);
	token = strtok(tmp, " ");
	
	for (int i=0; i<=count; i++) {
		strcpy(words[i]->word, token);
		token = strtok(NULL, " ");
	}


	if (!isspace(input[strlen(input)-2])) {
		words[count]->end = strlen(input);	
		strcat(output,"]");
	}
	printf("%s\n\n", output);

	for (int i=0; i<=count; i++) {
		printf("words[%d] = (%d, %d, %s)\n", i, words[i]->start, words[i]->end, words[i]->word);
	}


	for (int i=0; i<=count; i++) {
		free(words[i]);
	}


	return 0;

}
