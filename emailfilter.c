#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<regex.h>
#include"declarations.h"
#include<pthread.h>

void* runEmailFilter(void* arg){

	BufferManager bufman = (BufferManager) arg;

	for(;;){
		char *input;
    size_t inputsize = 50;
    size_t size_chars;

    input = (char *)malloc(inputsize * sizeof(char));
    if( input == NULL){
        perror("Unable to allocate buffer");
        exit(1);
    }

    size_chars = getline(&input,&inputsize,stdin);
		if(size_chars == EOF){
			EventInfo ei = malloc(sizeof(struct eventInfo));
			ei->action = "eof";
			ei->name = "eof";
			ei->date = "eof";
			ei->time = "eof";
			ei->location = "eof";
			addToBuffer(bufman, ei);
			pthread_exit(NULL);
		}
    input = strtok(input, "\n");
    
    regex_t regex;
    int reti;
    char msg[100];

    /* Compile regular expression */
    reti = regcomp(&regex, "^Subject:[[:blank:]][CDX][,][[:alnum:][:space:]]{10}[,](0[1-9]|1[012])[/](0[1-9]|[12][0-9]|3[01])[/](19|20)[[:digit:]][[:digit:]][,]([0-9]|0[0-9]|1[0-9]|2[0-3]):[0-5][0-9][,][[:alnum:][:space:]]{10}", 1);

    if (reti) {
      fprintf(stderr, "Could not compile regex\n");
      exit(1);
    }
		
    /* Execute regular expression */
    reti = regexec(&regex, input, 0, NULL, 0);

    if (!reti) {
      char subinput[41];
      memcpy( subinput, &input[9], 40 ); //removing prefix 'Subject'
      subinput[40] = '\0';
			
			char** tokens = str_split(subinput, ',');
			EventInfo ei = malloc(sizeof(struct eventInfo));
			ei->action = *tokens;
			ei->name = *(tokens + 1);
			ei->date = *(tokens + 2);
			ei->time = *(tokens + 3);
			ei->location = *(tokens + 4);

			addToBuffer(bufman, ei);
    }
    else if (reti == REG_NOMATCH) {
      //NOP
    }
    else {
      regerror(reti, &regex, msg, sizeof(msg));
      fprintf(stderr, "Regex match failed: %s\n", msg);
      exit(1);
    }

    /* Free memory allocated to the pattern buffer by regcomp() */
    regfree(&regex);
	}
}

