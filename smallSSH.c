// CS 344 Assignment 3: smallsh
// By Ashley Pettibone

// sources and inspirations
// used code from examples given by the instructor (I will label those in code)
// https://stackoverflow.com/ for trouble shooting and looking up how to o a specific task
// tutorialspoint.com


// to run the code
// gcc --std=gnu99 -o smallsh smallSSH.c
// ./smallsh

// to run with grading script
// chmod +x ./p3testscript
// ./p3testscript 2 > & 1


#include <stdio.h> 
#include <stdlib.h>  
#include <unistd.h>   
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_LIMIT 100


// counts total items in string
int itemCount = 0;

int commandCount = 0;
		
// user input
char userInput[MAX_LIMIT];
char *commands[MAX_LIMIT];


// var for expansion
char expansion[] = "$$";

// variables for comments and blank lines
char comment[] = "#";
char space[] = " ";


char *background;
char *fileIn;
char *fileOut;
char *argIn;
char *argOut;

char takIn[] = { "<" };
char outPut[] = { ">" };

// for loop for command prompt
int count = 2;

//sets exit status
int exitStatus;
 


/*
*
*/
void checkRedirection(){
	
	int i = 0;
	
	while(commands[i] != NULL) {
	//	printf("%s\n", commands[i]);
		if(strcmp(commands[i], takIn) == 0) {
			fileIn = commands[i];
			argIn = commands[i-1];
			printf("this is fileIn %s", commands[i]);
			printf("this is argIn %s", commands[i-1]);
		}
		if(strcmp(commands[i], outPut) == 0) {
			fileOut = commands[i+1];
			argOut = commands[i];
			printf("this is fileOut %s", commands[i+1]);
			printf("this is ardOut %s", commands[i]);
		}
		i++;
	}
}

//(strncmp(comment, commands[0], strlen(comment)) == 0)

/*
* This function changes directories
* if no argument is given after cd, chang to the home environment, otherwise chang to the directory given
*/
void changeDir() {
	// Your cd command should support both absolute and relative paths.
	// if not ./, then add that and chdir
		
	int x;
		
	if(commands[1] == NULL) {
	chdir(getenv("HOME"));
//		if(x == 0) {
//			printf("change was succesfull \n");
//		}
	}
	
	else {	
	
		//	int ch;
		//	ch = chdir(directory);
	// if commands doesn't doesn't start with ./-append to front 
		char *directory = commands[1];
		chdir(directory);
	
//		if(ch<0) {
//			printf("chdir change of directory NOT successful \n");
//		}
//		else {
//			printf("change was succesfull \n");
//		}
	}	
}


/*
*	prints the exit status or the terminating signal of the last foreground process
*/
void status(int exitVal) {
	
	int statusValue;
	statusValue = 0;

	statusValue = exitVal;

//If this command is run before any foreground command is run, then it should simply return the exit status 0.
//The three built-in shell commands do not count as foreground processes for the purposes of this built-in command - i.e., status should ignore built-in commands.
	printf("exit value %d\n", statusValue);
	fflush(stdout);
}


/*
* kills all process and then exits the program
*/
void exitProg() {
	//kill all processs and then exit
//	printf("this is in the exit function");
	int exitOn =0;
	
	exitOn = exitStatus;
	
	exit(exitOn);
}


// most of this code came from the examples the instructor gave us in the lecture
/*
*	runs the other commands
*/
void execCommands() {

	int childStatus;

	// Fork a new process
	pid_t spawnPid = fork();
  
	char *process;
  
	process = commands[0];

  switch(spawnPid){
    case -1:
      perror("fork()\n");
      exit(1);
      break;
    case 0:
      // In the child process
  //    printf("CHILD(%d) running ls command\n", getpid());
  
  	  // pass the given argument to exec function
      execlp(process, process, NULL);
      // exec only returns if there is an error
      perror("execlp");
      exit(EXIT_FAILURE);
      break;
    default:
      // In the parent process
      // Wait for child's termination
      spawnPid = waitpid(spawnPid, &childStatus, 0);
    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
      break;
  }
}

/*
* this checks if the argument given is a builtin command or a commented or blank line
* if not builtin, comment line, or blank line, passes command to exec
*/
void BuiltInCommands() {
	
	char cd[] = "cd";
	char stats[] = "status";
	char exitProgram[] = "exit";
	char echo1[]= "echo"; 
	
	// if input is #, then just reprompt
	if(strncmp(comment, commands[0], strlen(comment)) == 0) {
		
	}
	// if input is a space, also just reprompt
	else if(strncmp(space, commands[0], strlen(space)) == 0){
		
	}
	// otherwise check if the command is cd, if so change directories
	else if(strcmp(commands[0], cd) == 0) {
//		printf("this is cd");
		changeDir();	
	}
	// otherwise check if command is status, if so run status function
	else if(strcmp(commands[0], stats) == 0) {
//		printf("this is status");
		status(1);
	}
	// otherwise check if command is exit function, if so, exit
	else if(strcmp(commands[0], exitProgram) == 0) {
//		printf("this is exit");
		count = 1;
		exitProg();
	}
	else if (strcmp(commands[0], echo1) == 0) {
		
	}
	// if it's not a built in command or a comment or blank line, it must be another function, try passing to exec
	else {
		// its a dfferent command and pass it to execv
	//	execCommands();
	}
}


/*
* parses the user input from user
* puts user input in pointer array to be referenced later
* also counts commands while readin them in to know how many were passed in by the user
*/
void *parseCommand(char *currLine)
{
//	struct instructions *currItem = malloc(sizeof();

       
	int comCount=0;
	commandCount = 0;
	int commandSize = 0;
	
   // Extract the first token
	char *looptoken = strtok(currLine, " ");
	
//	char *point = strstr(looptoken, expansion);	
//		
//	// this means there is expansion to be done
//	if(point != NULL) {
//			
//	//	printf("point isn't null, this is pid: %d \n", getpid());
//		char expandCommand[MAX_LIMIT];
//		commandSize = (strlen(looptoken) - 2);
//		strncpy(expandCommand, looptoken, commandSize);
//		strcpy(looptoken, expandCommand);
//		// maybe need to do getppid;
//		sprintf(expandCommand, "%d", getpid());
//		strcat(looptoken, expandCommand);	
//	}
	
   // loop through the string to extract all other tokens 
	while(looptoken != NULL ) {
    //  printf( " %s\n", looptoken ); //printing each token
    
    	// put items in pointer array to be referenced later
    	commands[comCount++] = looptoken;
    	looptoken = strtok(NULL, " ");
    	
    	
    	
    	char *point1 = strstr(looptoken, expansion);	
		
		// this means there is expansion to be done
		if(point1 != NULL) {
			
		//	printf("point isn't null, this is pid: %d \n", getpid());
			char expandCommand[MAX_LIMIT];
			commandSize = (strlen(looptoken) - 2);
			strncpy(expandCommand, looptoken, commandSize);
			strcpy(looptoken, expandCommand);
			// maybe need to do getppid;
			sprintf(expandCommand, "%d", getpid());
			strcat(looptoken, expandCommand);	
		}
      
      	// cuont for number of cammands entered
        commandCount++;
   }
}


/*
* This starts the prompt and displays : for the user
* gets user input hands it off to be parsed
*/
void commandPrompt() {
	
	char *userCommand = malloc(sizeof(userCommand));
	
//	buffer = (char *)malloc(bufsize * sizeof(char));
	
	int commandSize1;
	
	
	while(count != 1){
		printf(": ");
		fflush(stdout);
		char newLine[] = "\n";	
		
		// get user input
		fgets(userInput, MAX_LIMIT, stdin); 
		
		// strip the newline
		commandSize1 = strlen(userInput);
		if(userInput[commandSize1-1] == '\n' )
		   	userInput[commandSize1-1] = 0;

		

//		char *point = strstr(userInput, expansion);	
//		
//		// this means there is expansion to be done
//		if(point != NULL) {
//			
//		//	printf("point isn't null, this is pid: %d \n", getpid());
//			char expandCommand[MAX_LIMIT];
//			commandSize = (strlen(userInput) - 2);
//			strncpy(expandCommand, userInput, commandSize);
//			strcpy(userInput, expandCommand);
//			// maybe need to do getppid;
//			sprintf(expandCommand, "%d", getpid());
//			strcat(userInput, expandCommand);	
//		}
	
			// parse the given command
			userCommand = parseCommand(userInput);
			
		// check if entire argument contains < or > 
		// if so, check redir 
		// fork from there
			checkRedirection();
			
			// if commands at 0 contains cd or status or exit
			// check builtin commands	
			BuiltInCommands();
			
		//	else
		// fork	
		
		//check & is at the end
	}
}



int main(){
	
	// displays the command prompt
	commandPrompt();

}


