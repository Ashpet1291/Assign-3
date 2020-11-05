// CS 344 Assignment 3: smallsh
// By Ashley Pettibone

// sources and inspirations
// used code from examples given by the instructor (I will label those in code)
// https://stackoverflow.com/ for trouble shooting and looking up how to o a specific task


#include <stdio.h>  // for perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LIMIT 60

struct instructions {
	char *command;
	char *arguments;
	char *redirIn;
	char *inputFile;
	char *redirOut;
	char *outputFile;
	char *runBack;
};

// counts total items in string
int itemCount = 0;

int commandCount = 0;
		
// user input
char userInput[MAX_LIMIT];

char *commands[MAX_LIMIT];

FILE *fp;

// var for expansion
char expansion[] = "$$";

// variables for comments and blank lines
char comment[] = "#";
char space[] = " ";



char *buffer;
    size_t bufsize = 32;
    size_t characters;
// used to clear the buffer 
        // and accept the next string 
 //       fflush(stdin); 
 
int exitProgra = 2;
 


//   The cd command changes the working directory of smallsh.
//   By itself - with no arguments - it changes to the directory specified in the HOME environment variable
//   This is typically not the location where smallsh was executed from, unless your shell executable is located in the HOME directory, in which case these are the same.
//   This command can also take one argument: the path of a directory to change to. Your cd command should support both absolute and relative paths.

/*
*
*/
void changeDir() {
	// code in program, if user command is cd followed by file, or file descriptor, try to change the directory
	// if chdir or fchdir doesn't fail then change directories, else cant change directories
	// if not ./, then add that and chdir
	
		
//	char *directory = "./happy";
//	char *homeDir = getenv("HOME");
	
	
//	int ch;
	
//	if(userComm->command != NULL) {
//		
//	}
//	else
//		ch = chdir(directory);
//	
//	if(ch<0) {
//		printf("chdir change of directory NOT successful \n");
//	}
//	else {
//		printf("change was succesfull \n");
//	}
	
	
	int x =0;
		
	if(commands[1] == NULL) {
	x =	chdir(getenv("HOME"));
		if(x == 0) {
			printf("change was succesfull \n");
		}
//		printf("%d\n", args);
	}
	
	else {	
	
		int ch = 0;
//	ch = chdir(directory);
	//	printf("%d\n", args);
		
			
		char *directory = "./happy";
		ch = chdir(directory);
	
		if(ch<0) {
			printf("chdir change of directory NOT successful \n");
		}
		else {
			printf("change was succesfull \n");
		}
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

/*
*
*/
void status(int exitVal) {
	
	int statusValue;
	statusValue = 0;
//	printf("need to add to this, status");
	statusValue = exitVal;
//The status command prints out either the exit status or the terminating signal of the last foreground process ran by your shell.
//	printf("this is in the status command");
//If this command is run before any foreground command is run, then it should simply return the exit status 0.
//The three built-in shell commands do not count as foreground processes for the purposes of this built-in command - i.e., status should ignore built-in commands.
	printf("exit value %d\n", statusValue);
	fflush(stdout);
}


/*
*
*/
void exitProg() {
	//kill all processs and then exit
//	printf("this is in the exit function");
	exit(0);
}

void execCommands() {
	
}


void BuiltInCommands() {
	
	char cd[] = "cd";
	char stats[] = "status";
	char exitProgram[] = "exit";
	
	if(commands[0] == NULL){
	//	exitProgra = 1;
	}
	//	may have to use string compare to compare
	if(strcmp(commands[0], cd) == 0) {
//		printf("this is cd");
		changeDir();	
	}
	else if(strcmp(commands[0], stats) == 0) {
//		printf("this is status");
		status(1);
	}
	else if(strcmp(commands[0], exitProgram) == 0) {
//		printf("this is exit");
		exitProgra = 1;
		exitProg();
	}
	else {
		// its a dfferent command and pass it to execv
	}
}


/*
*
*/
void parseCommand(char *currLine)
{
	struct instructions *currItem = malloc(sizeof(struct instructions));

    // For use with strtok_r
    char *saveptr;

       
	int comCount=0;
	commandCount = 0;
//   char string[50] = "Hello! We are learning about strtok";
   // Extract the first token
	char *looptoken = strtok(currLine, " ");
   // loop through the string to extract all other tokens
   
  
   
   
	while(looptoken != NULL ) {
    //  printf( " %s\n", looptoken ); //printing each token
    	commands[comCount++] = looptoken;
    	looptoken = strtok(NULL, " ");
      
        commandCount++;
   }
		
		
//	BuiltInCommands();	
		
//		int main ()
//{
//    char buf[] ="abc/qwe/ccd";
//    int i = 0;
//    char *p = strtok (buf, "/");
//    char *array[3];
//
//    while (p != NULL)
//    {
//        array[i++] = p;
//        p = strtok (NULL, "/");
//    }
//
//    for (i = 0; i < 3; ++i) 
//        printf("%s\n", array[i]);
//
//    return 0;


 //  char *looptoken = strtok(currLine, " ");
   // loop through the string to extract all other tokens
   
//   commandCount = 0;
//   while(looptoken != NULL ) {
//    //  printf( " %s\n", looptoken ); //printing each token
//      looptoken = strtok(NULL, " ");
//      commandCount++;
//   }

}


/*
*
*/
void commandPrompt() {
	
	struct instructions *userCommand = malloc(sizeof(struct instructions));
	
	buffer = (char *)malloc(bufsize * sizeof(char));
	
	int commandSize;
	
	
	while(exitProgra != 1){
		printf(": ");
		fflush(stdout);
		char newLine[] = "\n";	
		
		
		fgets(userInput, MAX_LIMIT, stdin); 
		
		
		commandSize = strlen(userInput);
		if(userInput[commandSize-1] == '\n' )
		   	userInput[commandSize-1] = 0;

		
		
//		// strip the newline form the input
//		char str[80];
//		int len;
//		fgets(str,sizeof(str),fp);
//		// remove newline
//		len = strlen(str);
//		if( str[len-1] == '\n' )
//    	str[len-1] = 0;
		char *point = strstr(userInput, expansion);	
		
		// this means there is expansion to be done
		if(point != NULL) {
			
		//	printf("point isn't null, this is pid: %d \n", getpid());
			char expandCommand[MAX_LIMIT];
			commandSize = (strlen(userInput) - 2);
			strncpy(expandCommand, userInput, commandSize);
			strcpy(userInput, expandCommand);
			// maybe need to do getppid;
			sprintf(expandCommand, "%d", getpid());
			strcat(userInput, expandCommand);	
		}
	
		if(strncmp(comment, userInput, strlen(comment) == 0) || (strncmp(space, userInput, strlen(space) == 0))) {
    
		}
		
		else
	//	if(point != NULL) {
	//	printf("%d", getpid());
	//	}
	
		parseCommand(userInput);
		//printf("This is comment %s", comment);
		//printf("this is space%s:", space);
		//puts(userInput);
		// checking for string and comments
		
	//	printf("usercommand command %s", userCommand->command);
		
		//check if usrInput contains $$
		//check & is at the end
	//		BuiltInCommands(userCommand);
		
		// check if userCommand is one of the builtIns cd, status, exit, if so go to builtIns(userCommand), else fork and got to all others
	}
}



int main(){
	

	commandPrompt();

}


