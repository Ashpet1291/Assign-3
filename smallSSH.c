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


int count = 2;

int exitStatus;


int inPresent = 0;
int outPresent = 0;
int bothPresent = 0;


/*
* This checks if file redirection is included in the arguments, if so it passes the args to an exec function
*/
void checkRedirection(){
	
	int i = 0;
	
	while(commands[i] != NULL) {
	//	printf("%s\n", commands[i]);
		if(strcmp(commands[i], takIn) == 0) {
			fileIn = commands[i+1];
			argIn = commands[i-1];
			inPresent = 1;
		//	printf("this is fileIn %s", commands[i+1]);
		//	printf("this is argIn %s", commands[i-1]);
		}
		if(strcmp(commands[i], outPut) == 0) {
			fileOut = commands[i+1];
			argOut = commands[i-1];
			outPresent = 1;
		//	printf("this is fileOut %s", commands[i+1]);
		//	printf("this is ardOut %s", commands[i-1]);
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
	char absFile[] = { "./" }
	int x;
	
	char tempcomm[] = {"0"};
		
	if(commands[1] == NULL) {
	chdir(getenv("HOME"));
//		if(x == 0) {
//			printf("change was succesfull \n");
//		}
	}
	
	else {	
	
		//	int ch;
		//	ch = chdir(directory);
		if(strncmp(absFile, commands[1], strlen(absFile)) == 0) {
			char *directory = commands[1];
			chdir(directory);
		}
		else {
			// append ./ to change directories
			strcat(absFile, commands[1]);
			
			// now directory name gets argument plus ./ appended to front
			char *directory1 = absFile;
			chdir(directory1);
			//strcpy(tempcomm, absFile); 			
		}
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

// int in, out;
//  char *grep_args[] = {"grep", "Villanova", NULL};
//
//  // open input and output files
//
//  in = open("scores", O_RDONLY);
//  out = open("out", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
//
//  // replace standard input with input file
//
//  dup2(in, 0);
//
//  // replace standard output with output file
//
//  dup2(out, 1);
//
//  // close unused file descriptors
//
//  close(in);
//  close(out);
//
//  // execute grep
//
//  execvp("grep", grep_args);
  
  
  
  
//  
//  FILE *fp;
//   int c;
//  
//   fp = fopen("file.txt","r");
//   while(1) {
//      c = fgetc(fp);
//      if( feof(fp) ) { 
//         break ;
//      }
//      printf("%c", c);
//   }
//   fclose(fp);
//   
//   return(0);
  
  
void execCommandsFileRedir() {
	
	FILE *in;
	FILE *out;
	

	int childStatus1;

	// Fork a new process
	pid_t spawnPid = fork();
  
	char *process1;
  
	process1 = argIn;

  switch(spawnPid){
    case -1:
      perror("fork()\n");
      exit(1);
      break;
    case 0:
      // In the child process
  //    printf("CHILD(%d) running ls command\n", getpid());
  	  in = fopen(fileIn,"r");
  	  out = fopen(fileOut,"a");
  	  
  	  
  	  dup2(in, 0);
  	  
  	  dup2(out, 1);
  	  
  	  
  	  close(in);
      close(out);
  	  
  	  // pass the given argument to exec function
      execlp(process1, process1, NULL);
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
 
  
//void execCommandArgs() {
//
//	int childStatus;
//
//	// Fork a new process
//	pid_t spawnPid = fork();
//  
//	char *process;
//  
//	process = commands[0];
//
//  switch(spawnPid){
//    case -1:
//      perror("fork()\n");
//      exit(1);
//      break;
//    case 0:
//      // In the child process
//  //    printf("CHILD(%d) running ls command\n", getpid());
//  
//  	  // pass the given argument to exec function
//      execlp(process, process, NULL);
//      // exec only returns if there is an error
//      perror("execlp");
//      exit(EXIT_FAILURE);
//      break;
//    default:
//      // In the parent process
//      // Wait for child's termination
//      spawnPid = waitpid(spawnPid, &childStatus, 0);
//    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
//      break;
//  }
//}

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
		checkRedirection();
		
		if((inPresent == 1) && outPresent == 1) {
			// do exe with args for both
			printf("both input and output are in command- do exec and dup for both");
		}
		else if (inPresent == 1) {
			// do exec with one arg
			printf("Input exec and dup for in");
		}
		else if (outPresent == 1) {
			//do out with exec and 1 arg
			printf("Input exec and dup for out");
		}
		else {
			// do exec with no args
			printf("just do exec");
		}
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
	
   // Extract the first token
	char *looptoken = strtok(currLine, " ");
	
   // loop through the string to extract all other tokens 
	while(looptoken != NULL ) {
    //  printf( " %s\n", looptoken ); //printing each token
    
    	// put items in pointer array to be referenced later
    	commands[comCount++] = looptoken;
    	looptoken = strtok(NULL, " ");
      
      	// cuont for number of cammands entered
        commandCount++;
   }
   
   
   
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
     
}


/*
* This starts the prompt and displays : for the user
* gets user input hands it off to be parsed
*/
void commandPrompt() {
	
	char *userCommand = malloc(sizeof(userCommand));
	
//	buffer = (char *)malloc(bufsize * sizeof(char));
	
	int commandSize;
	
	
	while(count != 1){
		printf(": ");
		fflush(stdout);
		char newLine[] = "\n";	
		
		// get user input
		fgets(userInput, MAX_LIMIT, stdin); 
		
		// strip the newline
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
	
			// parse the given command
			userCommand = parseCommand(userInput);
			
			
		//	checkRedirection();
			// check builtin commands	
			BuiltInCommands();
		
		//check if usrInput contains $$
		//check & is at the end
	}
}



int main(){
	
	// displays the command prompt
	commandPrompt();

}


