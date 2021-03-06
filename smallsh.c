// CS 344 Assignment 3: smallsh
// By Ashley Pettibone

// sources and inspirations
// used code from examples given by the instructor (I will label those in code)
// https://stackoverflow.com/ for trouble shooting and looking up how to o a specific task
// tutorialspoint.com
// https://www.stev.org/post/cgethomedirlocationinlinux for home directory code, and to error check

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
#include <pwd.h>
#include <signal.h> 

#define MAX_LIMIT 100
#define MAX_CHARS 2048
#define MAX_ARGS 512


// counts total items in string
int itemCount = 0;
int commandCount = 0;
		
// user input
char userInput[MAX_LIMIT];
char *commands[MAX_LIMIT];


//char *background;
char *fileIn;
char *fileOut;
char *argIn;
char *argOut;


int background = 0;
int backgroundFlag = 0;

int inPresent = 0;
int outPresent = 0;
int bothPresent = 0;


int pidArray = 0;

struct sigaction SIGINT_action = {0};
struct sigaction SIGTSTP_action = {0};

void handle_SIGINT(int sig) 
{
	// write message?
	//   need to use write printf("terminated by signal %d\n", signal);
	char* termSig = "terminated by signal ";
	// append signal to end of message and print
	//	strcat(termSig, sig);
	write(STDOUT_FILENO, termSig, 50);
	kill(getpid(), SIGTERM);
} 

void handle_SIGTSTP(int sig) {
	if(background == 1) {
		background = 0;
		backgroundFlag = 1;
		char* fgMessage = "Entering foreground-only mode (& is ignored)\n";
		write(STDOUT_FILENO, fgMessage, 50);
		fflush(stdout);
	}
	else {
		background = 1;
		backgroundFlag = 0;
		char* bgMessage = "Exiting foreground-only mode \n";
		write(STDOUT_FILENO, bgMessage, 30);
		fflush(stdout);
	}

}

/*
* This checks if file redirection is included in the arguments
*/
void checkRedirection() {
	
	int i = 0;
	char takeIn[] = {"<"};
	char outPut[] = {">"};	
	inPresent =0;
	outPresent = 0;
	
	while(commands[i] != NULL) {
		// checks if taking in a file or redirecting file out
		if(strcmp(commands[i], takeIn) == 0) {
				fileIn = commands[i+1];
				argIn = commands[i-1];
				inPresent = 1;
		}
		
		if(strcmp(commands[i], outPut) == 0) {
				fileOut = commands[i+1];
				argOut = commands[i-1];
				outPresent = 1;
		}
		i++;
	}
}

/*
* This function is used to change directories
* if no argument is given after cd, change to the home environment, otherwise change to the directory given
* supports both absolute and relative paths.
*/
void changeDir() {

	char absFile[] = { "./" };
	// if there is no given directory, change to home directory				
	if(commands[1] == NULL) {			
		chdir(getenv("HOME"));
	}
	
	else {	
		// check path type, if absolute, change directories
		if(strncmp(absFile, commands[1], strlen(absFile)) == 0) {
			chdir(commands[1]);
		}
		// otherwise append ./ to change directories
		else {
			strcat(absFile, commands[1]);
			chdir(absFile);			
		}
	}	
}


/*
*	prints the exit status or the terminating signal of the last foreground process
*/
void status(int exitVal) {
	
//	int statusValue = 0;
//	statusValue = exitVal;
	printf("exit status %d\n", exitVal);
	fflush(stdout);
}


/*
* kills all process and then exits the program
*/
void exitProg() {
	exit(0);
}


// most of this code came from the examples the instructor gave us in the lecture
/*
*	runs the other commands
*  called if no file redirection
*/
void execCommands() {

	int childStatus;
	char cats[] = "cat";

	// Fork a new process
	int spawnPid = fork();
    
	char *process; 
	process = commands[0];

    switch(spawnPid){
      case -1:
      	perror("fork()\n");
      	exit(1);
      break;
    case 0:
    	  
    	// child process ignores SIGTSTP whether foreground or background process
		signal(SIGTSTP, SIG_IGN);
		
		// if running in the background ignore SIGINT, otherwise use the handler    
     	if(background == 1) {
      		signal(SIGINT, SIG_IGN);
		}
		else {
			// running in foreground so use handler
			signal(SIGINT, handle_SIGINT);
			
		// printf("terminated by signal %d\n", signal);		

		//	kill(spawnPid, SIGKILL);
		}
  	    if(strcmp(cats, commands[0]) == 0) {
  	  	
  	  	char *processCat; 	  	
  	  	processCat = commands[1];
  	  	execlp(process, process, processCat, NULL);	
		}
	    else {
	    commands[commandCount] = NULL;
  	    // pass the given argument to exec function
        execvp(process, commands);     
  	    }
        // exec only returns if there is an error
        perror("execvp");
        exit(EXIT_FAILURE);
        break;
    default:
		// In the parent process
		// Wait for child's termination
        // parent ignores SIGINT   
		signal(SIGINT, SIG_IGN);
		// parent uses custom handler for SIGTSTP
        signal(SIGTSTP, handle_SIGTSTP);
		// if background is false -we're not running in the background, then wait on child process
        if(background == 0) {
      		waitpid(spawnPid, &childStatus, 0);
      	//	printf("background: %d", background);
	  }
	  else if (backgroundFlag == 1) {
	  	waitpid(spawnPid, &childStatus, 0);
	  }
	  else {
	  	// it is a background process, so don't wait and print process id
		printf("background pid is: %d\n", spawnPid);
		fflush(stdout); 
	//	printf("background: %d", background);		
			
	   	waitpid(spawnPid, &childStatus, WNOHANG);
	   	
	   	if (WIFEXITED(childStatus)) 
            printf("background pid %d is done: exit value: %d\n", 
                   spawnPid, WEXITSTATUS(childStatus));
				   	fflush(stdout); 
      } 
      background = 0;
      break;
  }
//   background = 0;
   memset(commands, '\0', sizeof(commands));
}


void execCommandsFileRedir() {
	
	FILE *in;
	FILE *out;

	int childStatus1;

	// Fork a new process
	int spawnPid = fork();
  	
  	/////////////////// add this pid to array
	// pidArray = spawnPid;
  
	char *process1;
  
	process1 = argIn;

    switch(spawnPid){
      case -1:
      	perror("fork()\n");
      	exit(1);
      	break;
      case 0:
    	
      // child process ignores SIGTSTP whether foreground or background process
	  signal(SIGTSTP, SIG_IGN);     
    
      // if running in the background ignore SIGINT, otherwise use the handler 
	  if(background == 1) {
     	signal(SIGINT, SIG_IGN);
	  }
	  else {
	  	// process is running in foreground so use handler
		signal(SIGINT, handle_SIGINT);
//			signal(SIGQUIT, SIG_IGN);
	//	kill(spawnPid, SIGKILL);
	  } 
      // In the child process
  //    printf("CHILD(%d) running ls command\n", getpid());
  	  in = fopen(fileIn,"r");
  	  
  	  int fI= fileno(in);
  	  	  
  	  out = fopen(fileOut,"a");
		  	  
 	  int fO = fileno(out);
  	  	  
  	  dup2(fI, 0);
  	  
  	  dup2(fO, 1);
   	  fclose(in);
      fclose(out);
  	  
  	  // pass the given argument to exec function
      execlp(process1, process1, NULL);
      
      // exec only returns if there is an error
      perror("execlp");
      exit(EXIT_FAILURE);
      break;
    default:    
      
        // parent ignores SIGINT   
		signal(SIGINT, SIG_IGN);
		// parent uses custom handler for SIGTSTP
        signal(SIGTSTP, handle_SIGTSTP);
       
	    // In the parent process
      // Wait for child's termination 
      if(background == 0) {
      	waitpid(spawnPid, &childStatus1, 0);
	  }
	  /////////////////////////////////// added this in for signals
	  // background commands are ignored
	  else if (backgroundFlag == 1) {
	  	waitpid(spawnPid, &childStatus1, 0);
	  }
	  ///////////////////////// may need to delete
	  
	  //otherwise it's a background process and work on it, but give control back to user for other processes
	  else {
	  	printf("pid is: %d\n", spawnPid);
	  	fflush(stdout);
	   	waitpid(spawnPid, &childStatus1, WNOHANG);
	   	
    } 
   	if (WIFEXITED(childStatus1)) 
            printf("background pid %d is done: exit value: %d\n", 
                   spawnPid, WEXITSTATUS(childStatus1));
				   	fflush(stdout); 
     // spawnPid = waitpid(spawnPid, &childStatus1, 0);
    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
     background = 0;
      break;
  }
  memset(commands, '\0', sizeof(commands));
}

//memset(pidArray,  '\0', sizeof(pidArray));

void execCommandsFileredirect() {
	
	FILE *in;
	FILE *out;

	int childStatus2;

	// Fork a new process
	int spawnPid = fork();
  
	// pidArray = spawnPid;
  
	char *process2;
	char *process3;
  
	process2 = argIn;
	process3 = argOut;

  switch(spawnPid){
  	
    case -1:
      perror("fork()\n");
      exit(1);
      break;
    case 0:
        // In the child process
      
        // child process ignores SIGTSTP whether foreground or background process
		signal(SIGTSTP, SIG_IGN);
		
		// if running in the background ignore SIGINT, otherwise use the handler 
        if(background == 1) {
      		signal(SIGINT, SIG_IGN);
		}
		else {
			// process is running in foregound so use handler
			signal(SIGINT, handle_SIGINT);
		//	signal(SIGQUIT, SIG_IGN);
		//	kill(spawnPid, SIGKILL);
		}
		
      if(inPresent == 1) {
      	// printf("CHILD(%d) running ls command\n", getpid());
    
      	// check if file exist, if so open, else print error
  	  	if(in = fopen(fileIn,"r")) { 	  		
  	  		int fI= fileno(in);
  	  		dup2(fI, 0);  	    	  
  	 		fclose(in);
  			// this error checks dup2    	
			// if(dup2(fd2[0], STDIN_FILENO) == -1){
			// perror("dup2");
			// return 1;
  	  		// pass the given argument to exec function
      		execlp(process2, process2, NULL);
        }
        
        // print error message because file doesn't exist
      	else {
      	//	printf("file doesn't exist\n");
      	//	fflush(stdout);
	    }
	  }
	  
	    else {
	  	
	  		out = fopen(fileOut,"a");
  	  
  	  		int fO = fileno(out);
  	  	
  	  		dup2(fO, 1);
  	  	
  	  		fclose(out);
  	  		  	
  	  	// pass the given argument to exec function
      	execlp(process3, process3, NULL);
	   }
	  	
 	  
//  	  // pass the given argument to exec function
//      execlp(process2, process2, NULL);
      // exec only returns if there is an error
      perror("execlp");
      exit(EXIT_FAILURE);
    //  exit(EXIT_FAILURE);
      break;
    default:
    
         // parent ignores SIGINT   
		signal(SIGINT, SIG_IGN);
		// parent uses custom handler for SIGTSTP
        signal(SIGTSTP, handle_SIGTSTP);
	
      // In the parent process
      // Wait for child's termination
      // if it's a foreground process-wait for it to finish
      if(background == 0) {
      	
      //	printf("background pid %d is done: terminated by signal %d", getpid());
      	waitpid(spawnPid, &childStatus2, 0);
	  }
	  else if (backgroundFlag == 1) {
	  	waitpid(spawnPid, &childStatus2, 0);
	  }
	  //otherwise it's a background process and work on it, but give control back to user for other processes
	  else {
	    printf("pid is: %d", spawnPid);
	   	fflush(stdout);	   	
	   	waitpid(spawnPid, &childStatus2, WNOHANG);
	   	
	
	   	if (WIFEXITED(childStatus2)) 
            printf("background pid %d is done: \n exit value: %d\n", 
                   spawnPid, WEXITSTATUS(childStatus2));
				   	fflush(stdout); 
      } 
     // spawnPid = waitpid(spawnPid, &childStatus2, 0);
    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
     background = 0;
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
	
	// variables for comments and blank lines
	char comment[] = "#";
	char space[] = " "; 
	
	// if input is #, then just reprompt
	if(strncmp(comment, commands[0], strlen(comment)) == 0) {
		
	}
	// if input is a space, also just reprompt
	else if(strncmp(space, commands[0], strlen(space)) == 0){
		
	}
	// otherwise check if the command is cd, if so change directories
	else if(strcmp(commands[0], cd) == 0) {
		changeDir();	
	}
	// otherwise check if command is status, if so run status function
	else if(strcmp(commands[0], stats) == 0) {
		status(1);
	}
	// otherwise check if command is exit function, if so, exit
	else if(strcmp(commands[0], exitProgram) == 0) {
		exitProg();
	}
	// if it's not a built in command or a comment or blank line, it must be another function, try passing to exec
	else {
		// its a dfferent command and pass it to execv
		// checks file redirection
		checkRedirection();
		if((inPresent == 1) && outPresent == 1) {
			// do exec with args for both
			execCommandsFileRedir();
		}
		else if ((inPresent == 1) || (outPresent == 1)){
			// do exec with one arg
			execCommandsFileredirect();
		}
		else {
			execCommands();
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
      
	int comCount=0;
	commandCount = 0;
	
	// Extract the first token
	char *looptoken = strtok(currLine, " ");
	
    // loop through the string to extract all other tokens 
	while(looptoken != NULL ) {
    
    	// put items in pointer array to be referenced later
    	commands[comCount++] = looptoken;
    	looptoken = strtok(NULL, " ");
      
      	// count for number of cammands entered
        commandCount++;
	}
}


/*
* This starts the prompt and displays : for the user
* gets user input and hands it off to be parsed
*/
void commandPrompt() {
	
	char *userCommand = malloc(sizeof(userCommand));
	char shpid[] = {"0"};
	
	char expansion[] = "$$";
	
	int commandSize;
	int len =0;
	
	while(1){
		printf(": ");
		fflush(stdout);	
		
		// get user input
		fgets(userInput, MAX_LIMIT, stdin); 
		
		// strip the newline
		commandSize = strlen(userInput);
		if(userInput[commandSize-1] == '\n' )
		   	userInput[commandSize-1] = 0;
		
		// check for expansion
		char *point = strstr(userInput, expansion);	
			
		char expandCommand[MAX_LIMIT];
		while(point != NULL) {
			
		//	printf("point isn't null, this is pid: %d \n", getpid());		
			memset(expandCommand, '\0', MAX_LIMIT);
			
			strncpy(expandCommand, userInput, strlen(userInput) - 2);
			
			memset(userInput, '\0', strlen(userInput));
			
			strcpy(userInput, expandCommand);
						
			sprintf(shpid, "%d", getppid());
			strcat(userInput, shpid);	
			
			memset(shpid, '\0', strlen(shpid));
	
			point = strstr(userInput, expansion);
	
		}
		
		// check status if ran before any other forground commands
		char *tmptr = strstr(userInput, "status");
		
		// it's a built in command, no need to check for background processes
		if(tmptr != NULL) {
			
			// parse the given command
			userCommand = parseCommand(userInput);						
			checkRedirection();
			// check builtin commands	
			BuiltInCommands();
		}
		else {
			
			int result = 0;
			
			// check if command given contains & at the end, if so thats a background process
			if((len = strlen(userInput)) > 1 && !strcmp(userInput + len - 1, "&")) {

			// then strip the background char to feed the command where it goes
			userInput[len-1] = 0;
			background = 1;
		//	printf("background this : %d\n", background);
		}

			// parse the given command
			userCommand = parseCommand(userInput);
						
			checkRedirection();
			// check builtin commands	
			BuiltInCommands();
		}	
	}
}

//If the user doesn't redirect the standard input for a background command, then standard input should be redirected to /dev/null
//If the user doesn't redirect the standard output for a background command, then standard output should be redirected to /dev/null

int main(){

	// this code was mostly from the example the instructor provided
	// Fill out the SIGINT_action struct
  // Register handle_SIGINT as the signal handler
	SIGINT_action.sa_handler = SIG_IGN;
  // Block all catchable signals while handle_SIGINT is running
	sigfillset(&SIGINT_action.sa_mask);
  // No flags set
//////	SIGINT_action.sa_flags = 0;

  // Install our signal handler
	sigaction(SIGINT, &SIGINT_action, NULL);
	
	
	SIGTSTP_action.sa_handler = handle_SIGTSTP;
	
	sigfillset(&SIGTSTP_action.sa_mask);
  // No flags set
///////	SIGTSTP_action.sa_flags = 0;

  // Install our signal handler
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
		
	commandPrompt();

}


