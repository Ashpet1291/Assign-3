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

#define MAX_LENGTH 80
#define MAX_ARGS 51
#define STR_MAX 60



int commandCount = 0;
		
// user input
char *userInput[MAX_LENGTH];
//char *userInputString[MAX_ARGS][STR_MAX];

char word[MAX_LENGTH];

// int *arr = (int *)malloc(r * c * sizeof(int)); 

// var for expansion
char expansion[] = "$$";

// variables for comments and blank lines
char comment[] = "#";
char space[] = " ";

int breakVal = 2;

int args = 0;

/*
*
*/
//void *parseCommand(char *instructions)
//{
////	struct instructions *currItem = malloc(sizeof(struct instructions));
//
//
////char str[] = "Geeks for Geeks"; 
////    char* token; 
////    char* rest = str; 
////  
////    while ((token = strtok_r(rest, " ", &rest))) 
////        printf("%s\n", token); 
////  
////    return (0); 
////} 
//    // For use with strtok_r
//    
//    
//    
//    
//    
////  int argGather(char*args) {
////    int counter = 0; // remeber to +1 for printing out since it is at 0
////    printf("%s", ": "); // prompt beginning
////    fflush(stdout);
////    fgets(args,2048,stdin); // read in what the user wants
////    strtok(args, "\n"); // removing the newlines from getline
////    char buffer [2048];
////    int i, j;
////   
////    char *word = strtok(args, " "); // spliting the string to get every word
////    
////    // using tokens to split up the string
////    while (word != NULL) {  // token word that will iterated
////        argList[counter] = word;
////        word = strtok(NULL, " ");  // move to next word 
////        counter++;
////    }
//
////   char string[50] = "Hello! We are learning about strtok";
//   // Extract the first token
//   char *looptoken = strtok(instructions, " ");
//   // loop through the string to extract all other tokens 
//   commandCount = 0;
//   while(looptoken != NULL ) {
//    //  printf( " %s\n", looptoken ); //printing each token
//      looptoken = strtok(NULL, " ");
//      commandCount++;
//   }		
//	// prints number of commnads
////	printf("%d", commandCount);
//
// 	char *saveptr;  
//    // The first token is the command
//    int i = 0;
//    int j = 0;
//    
//    char *token = strtok_r(instructions, " ", &saveptr);
//    
//    strcpy(userInputString[i][STR_MAX], token);
//    i++;
//    
//    while(token != NULL ) {
//    	
//    char *token = strtok_r(instructions, " ", &saveptr);
//    
//    strcpy(userInputString[i][STR_MAX], token);
//    i++;
//	}
//	
//	
//	for(i=0 ;i<commandCount ;i++)
//		printf("%s\n",userInputString[i][STR_MAX]);		
//	
//}

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
	int x =0;
		
	if(userInput[1] == NULL) {
	x =	chdir(getenv("HOME"));
		if(x == 0) {
			printf("change was succesfull \n");
		}
//		printf("%d\n", args);
	}
	
	else {	
	
		int ch = 0;
//	ch = chdir(directory);
		printf("%d\n", args);
		
			
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




//    else{ // if there are more than one argument, only will read the the first after chdir
//        if ( chdir(argList[1]) != 0 ){
//        }
//    }



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


/*
*
*/
void BuiltInCommands() {
	
	char cd[] = "cd";
	char stats[] = "status";
	char exitProgram[] = "exit";
	
	if(userInput[0] == NULL){
	//	exitProgra = 1;
	}
	//	may have to use string compare to compare
	if(strcmp(userInput[0], cd) == 0) {
//		printf("this is cd");
		changeDir();	
	}
	else if(strcmp(userInput[0], stats) == 0) {
//		printf("this is status");
		status(1);
	}
	else if(strcmp(userInput[0], exitProgram) == 0) {
	//	printf("this is exit");
		exitProg();
	}
	else {
		// its a dfferent command and pass it to execv
	}
}


//void printargs() {
//	// print all values
//	for(int k=0; k<args; k++) {
//		printf("%s\n", userInput[k]);
//	}	
//}

void argCount() {
	for(int k=0; k<args; k++) {
			commandCount++;
	}
	printf("%d\n", commandCount);
}

/*
*
*/
void commandPrompt() {
	
	char *userCommand; //= malloc(sizeof(userCommand));
	
  //	buffer = (char *)malloc(bufsize * sizeof(char));
	int i;
	
	int commandSize;

	
	while(breakVal != 1){
		
		args = 0;
		
		printf(": ");
		fflush(stdout);
		char newLine[] = "\n";	
		
		
		fgets(word, MAX_LENGTH, stdin); 
	//	printf("string is: %s\n", userInput);
		
		
		
		unsigned len =0;
		
		
		len = strlen(word);
		word[len-1] = '\0';
		userCommand = (char*) malloc(len);
		strcpy(userCommand, word);
		userInput[args++]=userCommand;
		
		
		printf("%s %s", userInput[0]);
		
//		argCount();
		
		BuiltInCommands();
		
//		printargs();
	}



// print all values
//		for(int k=0; k<args; k++) {
//			printf("%s\n", userInput[k]);
//		}
		
//		commandSize = strlen(userInput);
//		if(userInput[commandSize-1] == '\n' )
//		   	userInput[commandSize-1] = 0;


////////////////expansion
//		char *point = strstr(userInput, expansion);
//	





////////////////break up tokens	
//		char temp[2048];
//		
//		
//		char *inputItem = strtok(userInput, " ");
//		
//		while(inputItem != NULL) {
//		userInput[i] = inputItem;
//		inputItem = strtok(NULL, " ");
//		i++;
//		commandCount++;
//		}
//		// strip the newline form the input
//		char str[80];
//		int len;
//		fgets(str,sizeof(str),fp);
//		// remove newline
//		len = strlen(str);
//		if( str[len-1] == '\n' )
//    	str[len-1] = 0;
			
		
		
		
		//  int argGather(char*args) {
//    int counter = 0; // remeber to +1 for printing out since it is at 0
//    printf("%s", ": "); // prompt beginning
//    fflush(stdout);
//    fgets(args,2048,stdin); // read in what the user wants
//    strtok(args, "\n"); // removing the newlines from getline
//    char buffer [2048];
//    int i, j;
//   
//    char *word = strtok(args, " "); // spliting the string to get every word
//    
//    // using tokens to split up the string
//    while (word != NULL) {  // token word that will iterated
//        argList[counter] = word;
//        word = strtok(NULL, " ");  // move to next word 
//        counter++;
//    }

//
 
    

		
		
		
		
//		
//		// this means there is expansion to be done
//		if(point != NULL) {
//			
//		//	printf("point isn't null, this is pid: %d \n", getpid());
//			char expandCommand[MAX_LENGTH];
//			commandSize = (strlen(userInput) - 2);
//			strncpy(expandCommand, userInput, commandSize);
//			strcpy(userInput, expandCommand);
//			// maybe need to do getppid;
//			sprintf(expandCommand, "%d", getpid());
//			strcat(userInput, expandCommand);	
//		}
//	
//		if(strncmp(comment, userInput, strlen(comment) == 0) || (strncmp(space, userInput, strlen(space) == 0))) {
//    
//		}
	//	else
	//	if(point != NULL) {
	//	printf("%d", getpid());
	//	}
	
//			userCommand = parseCommand(userInput);
		//printf("This is comment %s", comment);
		//printf("this is space%s:", space);
		//puts(userInput);
		// checking for string and comments
		
	//	printf("usercommand command %s", userCommand->command);
		
		//check if usrInput contains $$
		//check & is at the end
		//	BuiltInCommands(userCommand);
		
		// check if userCommand is one of the builtIns cd, status, exit, if so go to builtIns(userCommand), else fork and got to all others
	
}



int main(){
	

	commandPrompt();

}
