// CS 344 Assignment 3: smallsh
// By Ashley Pettibone

// sources and inspirations
// used code from examples given by the instructor (I will label those in code)
// https://stackoverflow.com/ for trouble shooting and looking up how to o a specific task



// to run the code
// gcc --std=gnu99 -o smallsh smallSSH.c
// ./smallsh

// to run with grading script
// chmod +x ./p3testscript
// ./p3testscript 2 > & 2



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



// used to clear the buffer 
        // and accept the next string 
 //       fflush(stdin); 
 
int exitstatus = 2;
 


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
	
	
	int x;
		
	if(commands[1] == NULL) {
	x =	chdir(getenv("HOME"));
//		if(x == 0) {
//		//	printf("change was succesfull \n");
//		}
//		printf("%d\n", args);
	}
	
	else {	
	
		int ch;
		//	ch = chdir(directory);
		//	printf("%d\n", args);
		
			
		char *directory = commands[1];
		ch = chdir(directory);
	
//		if(ch<0) {
//			printf("chdir change of directory NOT successful \n");
//		}
//		else {
//			printf("change was succesfull \n");
//		}
	}
	
	
	
	
	
}


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
    //  exit(1);
      break;
    case 0:
      // In the child process
  //    printf("CHILD(%d) running ls command\n", getpid());
      // Replace the current program with "/bin/ls"
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
* this checks if the argument given is a builtin command
* otherwise passes it to exec
*/
void BuiltInCommands() {
	
	char cd[] = "cd";
	char stats[] = "status";
	char exitProgram[] = "exit";
	char echo1[]= "echo"; 
	
	
//		if(strncmp(comment, userInput, strlen(comment) == 0) || (strncmp(space, userInput, strlen(space) == 0))) {
//    
//		}
//		
//		else
//	
	
	if(strncmp(comment, commands[0], strlen(comment)) == 0) {
		
	}
	//	exitProgra = 1;
	else if(strncmp(space, commands[0], strlen(space)) == 0){
		
	}
	//	may have to use string compare to compare
	else if(strcmp(commands[0], cd) == 0) {
//		printf("this is cd");
		changeDir();	
	}
	else if(strcmp(commands[0], stats) == 0) {
//		printf("this is status");
		status(1);
	}
	else if(strcmp(commands[0], exitProgram) == 0) {
//		printf("this is exit");
		exitstatus = 1;
		exitProg();
	}
	else if (strcmp(commands[0], echo1) == 0) {
		
	}
	else {
		// its a dfferent command and pass it to execv
		execCommands();
	}
}


/*
*	parses the user input from user
*/
void *parseCommand(char *currLine)
{
//	struct instructions *currItem = malloc(sizeof();

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
		
		
	BuiltInCommands();	
		
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
* This starts the prompt and display : for the user
* gets user input
*/
void commandPrompt() {
	
	char *userCommand = malloc(sizeof(userCommand));
	
//	buffer = (char *)malloc(bufsize * sizeof(char));
	
	int commandSize;
	
	
	while(exitstatus != 1){
		printf(": ");
		fflush(stdout);
		char newLine[] = "\n";	
		
		
		fgets(userInput, MAX_LIMIT, stdin); 
		
		
		commandSize = strlen(userInput);
		if(userInput[commandSize-1] == '\n' )
		   	userInput[commandSize-1] = 0;

		

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
	
	
			userCommand = parseCommand(userInput);
		
		//check if usrInput contains $$
		//check & is at the end
	//		BuiltInCommands(userCommand);
		
		// check if userCommand is one of the builtIns cd, status, exit, if so go to builtIns(userCommand), else fork and got to all others
	}
}



int main(){
	

	commandPrompt();

}


