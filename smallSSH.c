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



struct sigaction SIGINT_action = {0};
struct sigaction SIGTSTP_action = {0};


// counts total items in string
int itemCount = 0;

int commandCount = 0;
		
// user input
char userInput[MAX_LIMIT];
char *commands[MAX_LIMIT];

char *pidArray;


// var for expansion
char expansion[] = "$$";

// variables for comments and blank lines
char comment[] = "#";
char space[] = " ";


//char *background;
char *fileIn;
char *fileOut;
char *argIn;
char *argOut;

char takIn[] = { "<" };
char outPut[] = { ">" };


int count = 2;

int terminateProgram;
int background = 0;


int inPresent = 0;
int outPresent = 0;
int bothPresent = 0;


void handle_SIGINT(int signo){
	char* message = "Caught SIGINT\n";
  // We are using write rather than printf
	write(STDOUT_FILENO, message, 30);
	sleep(10);
}


void handle_SIGSTP(int signo){
	char* message = "Entering foreground-only mode (& is now ignored)\n";
	
	// If it's 1, set it to 0 and display a message reentrantly
	if (background == 1) {
		char* message = "Entering foreground-only mode (& is now ignored)\n";
		write(1, message, 49);
		fflush(stdout);
	//	background = 0;
	}

	// If it's 0, set it to 1 and display a message reentrantly
	else {
		char* message = "Exiting foreground-only mode\n";
		write (1, message, 48);
		fflush(stdout);
	//	allowBackground = 1;
	}
	
//	sleep(10);
}



/*
* This checks if file redirection is included in the arguments, if so it passes the args to an exec function
*/
void checkRedirection(){
	
	int i = 0;
	inPresent =0;
	outPresent = 0;
	
	FILE *checkFile;
	
	
	while(commands[i] != NULL) {
	//	printf("%s\n", commands[i]);
	
	
		if(strcmp(commands[i], takIn) == 0) {
//			if (checkFile = fopen(commands[i+1], "r")) {
//      			fclose(checkFile);
				fileIn = commands[i+1];
				argIn = commands[i-1];
				inPresent = 1;
//			}
//			else {
//				perror("File don't exist\n");
//			}
		//	printf("this is fileIn %s", commands[i+1]);
		//	printf("this is argIn %s", commands[i-1]);
		}
		if(strcmp(commands[i], outPut) == 0) {
//			if (checkFile = fopen(commands[i+1], "r")) {
//				fclose(checkFile);
				fileOut = commands[i+1];
				argOut = commands[i-1];
				outPresent = 1;
//			}
//			else {
//				perror("File don't exist\n");
//			}	
		//	printf("this is fileOut %s", commands[i+1]);
		//	printf("this is ardOut %s", commands[i-1]);
		}
		i++;
	}
}

///* try to open file to read */
//   FILE *file;
//   if (file = fopen("a.txt", "r")) {
//      fclose(file);
//      printf("file exists");
//   } else {
//      printf("file doesn't exist");
//   }
//if((len = strlen(str)) > 3 && !strcmp(str + len - 4, ".txt"))


//(strncmp(comment, commands[0], strlen(comment)) == 0)

/*
* This function changes directories
* if no argument is given after cd, chang to the home environment, otherwise chang to the directory given
*/
void changeDir() {
	// Your cd command should support both absolute and relative paths.
	// if not ./, then add that and chdir
	char absFile[] = { "./" };
	
	char tempcomm[] = {"0"};
		
	if(commands[1] == NULL) {
		
		
		chdir(getenv("HOME"));
		
//		char *homedir = getenv("HOME");
//
//        if (homedir != NULL) {
//        //        printf("Home dir in enviroment");
//        //        printf("%s\n", homedir);       
//    		chdir(homedir);
//        }
//
//        uid_t uid = getuid();
//        struct passwd *pw = getpwuid(uid);
//
//        if (pw == NULL) {
//                printf("cd HOME Failed\n");
//                exit(EXIT_FAILURE);
//        }
    //    printf("%s\n", pw->pw_dir);

	}
	
	else {	
	
		//	int ch;
		//	ch = chdir(directory);
		if(strncmp(absFile, commands[1], strlen(absFile)) == 0) {
		//	char *directory = commands[1];
			chdir(commands[1]);
		}
		else {
			// append ./ to change directories
			strcat(absFile, commands[1]);
			
			// now directory name gets argument plus ./ appended to front
		//	char *directory1 = absFile;
			chdir(absFile);
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
	char cats[] = "cat";

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
  	  
  	  	if(background == 1) {
      		signal(SIGINT, SIG_IGN);
		  }
      // exec only returns if there is an error
      perror("execvp");
      exit(EXIT_FAILURE);
      break;
    default:
      // In the parent process
      // Wait for child's termination
      
      if(background == 0) {
      	waitpid(spawnPid, &childStatus, 0);
	  }
	  //otherwise it's a background process and work on it, but gove control back to user for other processes
	  else {
	  	// it is a background process, so don't wait and print process id
		printf("background pid is: %d\n", spawnPid);
		fflush(stdout);
		// 
	   	waitpid(spawnPid, &childStatus, WNOHANG);
	   	
	   	if (WIFEXITED(childStatus)) 
            printf("background pid %d is done: exit value: %d\n", 
                   spawnPid, WEXITSTATUS(childStatus));
				   	fflush(stdout); 
      } 
      //spawnPid = waitpid(spawnPid, &childStatus, 0);
    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
    background = 0;
      break;
  }
  
  
  
}




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
  	  
  	  int fI= fileno(in);
  	  
  	  
  	  out = fopen(fileOut,"a");
  	  
  	  int fO = fileno(out);
  	  
  	  
  	  dup2(fI, 0);
  	  
  	  dup2(fO, 1);
  	  
  	  
  	  fclose(in);
      fclose(out);
  	  
  	  // pass the given argument to exec function
      execlp(process1, process1, NULL);
      
      
      	if(background == 1) {
      		signal(SIGINT, SIG_IGN);
		  }
      // exec only returns if there is an error
      perror("execlp");
      exit(EXIT_FAILURE);
      break;
    default:
      // In the parent process
      // Wait for child's termination
      
      if(background == 0) {
      	waitpid(spawnPid, &childStatus1, 0);
	  }
	  //otherwise it's a background process and work on it, but gove control back to user for other processes
	  else {
	  	printf("pid is: %d\n", spawnPid);
	  	fflush(stdout);
	   	waitpid(spawnPid, &childStatus1, WNOHANG);
	   	if (WIFEXITED(childStatus1)) 
            printf("background pid %d is done: exit value: %d\n", 
                   spawnPid, WEXITSTATUS(childStatus1));
				   	fflush(stdout); 
    } 
     // spawnPid = waitpid(spawnPid, &childStatus1, 0);
    //  printf("PARENT(%d): child(%d) terminated. Exiting\n", getpid(), spawnPid);
     background = 0;
      break;
  }
}



void execCommandsFileredirect() {
	
	FILE *in;
	FILE *out;
	

	int childStatus2;

	// Fork a new process
	pid_t spawnPid = fork();
  
	char *process2;
	char *process3;
  
	process2 = argIn;
	process3 = argOut;

  switch(spawnPid){
  	
  	
  //	char message[] = "terminated by signal %d"
  	
    case -1:
      perror("fork()\n");
      exit(1);
      break;
    case 0:
      // In the child process
      
      if(inPresent == 1) {
      	//    printf("CHILD(%d) running ls command\n", getpid());
      	

      	// check if file exist, if so open, else print error
  	  	if(in = fopen(fileIn,"r")) {
  	  		
  	  		int fI= fileno(in);
  	  
  	  		dup2(fI, 0);
  	    	  
  	 		fclose(in);
  // this error checks dup2    	
//  	  if(dup2(fd2[0], STDIN_FILENO) == -1){
//			perror("dup2");
//			return 1;

  	  		// pass the given argument to exec function
      		execlp(process2, process2, NULL);
        }
        
        // print error message because file doesn't exist
      	else {
      	//	printf("file doesn't exist\n");
      		fflush(stdout);
	    }
	  }
	  
	  else {
	  	
	  	out = fopen(fileOut,"a");
  	  
  	  	int fO = fileno(out);
  	  	
  	  	dup2(fO, 1);
  	  	
  	  	fclose(out);
  	  	
  	  	
  	  	// pass the given argument to exec function
      	execlp(process3, process3, NULL);
      	
      	
      	if(background == 1) {
      		signal(SIGINT, SIG_IGN);
		  }
      	
	  	
	  }
 	  
//  	  // pass the given argument to exec function
//      execlp(process2, process2, NULL);
      // exec only returns if there is an error
      perror("execlp");
      exit(EXIT_FAILURE);
    //  exit(EXIT_FAILURE);
      break;
    default:
      // In the parent process
      // Wait for child's termination
      // if it's a foreground process-wait for it to finish
      if(background == 0) {
      	
      //	printf("background pid %d is done: terminated by signal %d", getpid());
      	waitpid(spawnPid, &childStatus2, 0);
	  }
	  //otherwise it's a background process and work on it, but gove control back to user for other processes
	  else {
	   printf("pid is: %d", spawnPid);
	   	fflush(stdout);
	   	waitpid(spawnPid, &childStatus2, WNOHANG);
	   	
	   	if (WIFEXITED(childStatus2)) 
            printf("background pid %d is done: exit value: %d\n", 
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
	// if it's not a built in command or a comment or blank line, it must be another function, try passing to exec
	else {
		// its a dfferent command and pass it to execv
		checkRedirection();
		
		if((inPresent == 1) && outPresent == 1) {
			// do exe with args for both
			execCommandsFileRedir();
		//	printf("both input and output are in command- do exec and dup for both");
		}
		else if (inPresent == 1) {
			// do exec with one arg
		//	printf("Input exec and dup for in");
			execCommandsFileredirect();
		}
		else if (outPresent == 1) {
			//do out with exec and 1 arg
		//	printf("Input exec and dup for out");
			execCommandsFileredirect();
		}
		else {
			execCommands();
		}
//		execCommands();
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
   
//   
//   char *point1 = strstr(looptoken, expansion);	
//		
//		// this means there is expansion to be done
//		if(point1 != NULL) {
//			
//		//	printf("point isn't null, this is pid: %d \n", getpid());
//			char expandCommand[MAX_LIMIT];
//			commandSize = (strlen(looptoken) - 2);
//			strncpy(expandCommand, looptoken, commandSize);
//			strcpy(looptoken, expandCommand);
//			// maybe need to do getppid;
//			sprintf(expandCommand, "%d", getpid());
//			strcat(looptoken, expandCommand);	
//		}
   
    
}


/*
* This starts the prompt and displays : for the user
* gets user input hands it off to be parsed
*/
void commandPrompt() {
	
	char *userCommand = malloc(sizeof(userCommand));
	char shpid[] = {"0"};
	
//	buffer = (char *)malloc(bufsize * sizeof(char));
	
	int commandSize;
	int len =0;
	
	char builtIns[] = "cd, status, exit";
	
	
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

		
		// check for expansion
		char *point = strstr(userInput, expansion);	
		
		// this means there is expansion to be done
//		if(point != NULL) {
//			
//		//	printf("point isn't null, this is pid: %d \n", getpid());
//			char expandCommand[MAX_LIMIT];
//			
//			// lower the size by 2
//		//	commandSize = (strlen(userInput) - 2);
//		
//			// had command size in spot for size
//			// 
//			strncpy(expandCommand, userInput, strlen(userInput) - 2);
//			strcpy(userInput, expandCommand);
//			// maybe need to do getppid;
//			sprintf(shpid, "%d", getpid());
//			strcat(userInput, shpid);	
//			
//			memset(shpid, '\0', strlen(shpid));
//			
//		//	clears for cd, but then gets hung on -- Testing foreground-only mode--kill -SIGTSTP $$
//	//	//	memset(expandCommand, '\0', strlen(expandCommand));
//		}
		char expandCommand[MAX_LIMIT];
		while(point != NULL) {
			
		//	printf("point isn't null, this is pid: %d \n", getpid());
			
			memset(expandCommand, '\0', MAX_LIMIT);
			
			// lower the size by 2
		//	commandSize = (strlen(userInput) - 2);
		
			// had command size in spot for size
			// 
			strncpy(expandCommand, userInput, strlen(userInput) - 2);
			
			
			memset(userInput, '\0', strlen(userInput));
			
			strcpy(userInput, expandCommand);
			
		//	memset(expandCommand, '\0', strlen(expandCommand));
			
			// maybe need to do getppid;
			sprintf(shpid, "%d", getppid());
			strcat(userInput, shpid);	
			
			memset(shpid, '\0', strlen(shpid));
			
		//	clears for cd, but then gets hung on -- Testing foreground-only mode--kill -SIGTSTP $$
	//	//	memset(expandCommand, '\0', strlen(expandCommand));
	
			point = strstr(userInput, expansion);
	
		}
		
	//	printf("parent pid %d:  other pid %d: ", getpid(), getppid());
	
		
	
		char *tmptr = strstr(userInput, "status");
		
		// it's abuilt in command, no need to check for background processes
		if(tmptr != NULL) {
			
						// parse the given command
			userCommand = parseCommand(userInput);
			
			
			checkRedirection();
			// check builtin commands	
			BuiltInCommands();
		}
		else {
			
			// check if command given contains & at the end, if so thats a background process
			if((len = strlen(userInput)) > 1 && !strcmp(userInput + len - 1, "&")) {
		
		//	printf("this is background proc\n");
			// then strip the background char to feed the command where it goes
			userInput[len-1] = 0;
		//	printf("This is in the background");
			background = 1;
		}

			// parse the given command
			userCommand = parseCommand(userInput);
			
			
			checkRedirection();
			// check builtin commands	
			BuiltInCommands();
		}
		
	}
}

//Any non built-in command with an & at the end must be run as a background command and the shell must not wait for such a command to complete.
// For such commands, the parent must return command line access and control to the user immediately after forking off the child.

//The shell will print the process id of a background process when it begins.
//When a background process terminates, a message showing the process id and exit status will be printed. This message must be printed just before the prompt for a new command is displayed.
//If the user doesn't redirect the standard input for a background command, then standard input should be redirected to /dev/null
//If the user doesn't redirect the standard output for a background command, then standard output should be redirected to /dev/null

int main(){
	
//	
//	// SIGINT handling - default is ignore
//	SIGINT_action.sa_handler = SIG_IGN;
//	sigaction(SIGINT, &SIGINT_action, NULL);
//
//	// SIGTSTP handling - foreground only mode
//	SIGTSTP_action.sa_handler = handle_SIGSTP;
//	sigfillset(&SIGTSTP_action.sa_mask);
//	SIGTSTP_action.sa_flags = SA_RESTART;
//	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
	// displays the command prompt
	commandPrompt();

}


