#include <stdio.h>  // for perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv
#include <string.h>

#define MAX_LIMIT 60



// user input
char userInput[MAX_LIMIT];


FILE *fp;

char expansion[] = "$$";

char comment[] = "#";
char space[] = " ";

// basically throw away the line because it is a comment
char trash;

char *buffer;
    size_t bufsize = 32;
    size_t characters;
// used to clear the buffer 
        // and accept the next string 
 //       fflush(stdin); 


// exit(0)

void changeDir() {
	// code in program, if user command is cd followed by file, or file descriptor, try to change the directory
	// if chdir or fchdir doesn't fail then change directories, else cant change directories
	// if not ./, then add that and chdir
	
	int ch = chdir("/happy");
	if(ch == 0) {
		printf("chdir change was succesful \n");
	}
	else {
		printf("chdir change of directory NOT successful \n");
	}
}


//char *directory = "/tmp";
//int ret;
//
//ret = chdir (directory);


// //pass your path in the function
//    int ch=chdir("xxx");
//    /*if the change of directory was successful it will print successful otherwise it will print not successful*/
//    if(ch<0)
//    printf("chdir change of directory not successful\n");
//    else
//    printf("chdir change of directory successful");




int main(){
	
	 buffer = (char *)malloc(bufsize * sizeof(char));
//	while(userInput !=){
		printf(": ");
		fgets(userInput, MAX_LIMIT, stdin); 
   	//	printf("%s", userInput);	
//	}

	char *point = strstr(userInput, expansion);
	
	if(point != NULL) {
	//	printf("%d", getpid());
	}
	
	if((strncmp(comment, userInput, strlen(comment)) == 0) || (strncmp(space, userInput, strlen(space)) == 0)) {
    	trash = getline(&buffer,&bufsize,stdin);
	}
	changeDir();
//	}




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

