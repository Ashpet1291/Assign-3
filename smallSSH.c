#include <stdio.h>  // for perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv

#define MAX_LIMIT 60



// user input
char userInput[MAX_LIMIT];


FILE *fp;

char expansion[] = "$$";

char comment[] = "#";

// basically throw away the line because it is a comment
char trash;

char *buffer;
    size_t bufsize = 32;
    size_t characters;
// used to clear the buffer 
        // and accept the next string 
 //       fflush(stdin); 



int main(){
	
	 buffer = (char *)malloc(bufsize * sizeof(char));
//	while(userInput != -5){
		printf(": ");
		fgets(userInput, MAX_LIMIT, stdin); 
   		printf("%s", userInput);	
//	}

	char *point = strstr(userInput, expansion);
	
	if(point != NULL) {
		printf("%d", getpid());
	}
	
	if((strncmp(comment, userInput, strlen(comment)) == 0)) {
    	trash = getline(&buffer,&bufsize,stdin);
	}

}
//\\char *buffer;
//    size_t bufsize = 32;
//    size_t characters;
//
//    buffer = (char *)malloc(bufsize * sizeof(char));
//    if( buffer == NULL)
//    {
//        perror("Unable to allocate buffer");
//        exit(1);
//    }
//
//    printf("Type something: ");
//    characters = getline(&buffer,&bufsize,stdin);
//    printf("%zu characters were read.\n",characters);
//    printf("You typed: '%s'\n",buffer);

