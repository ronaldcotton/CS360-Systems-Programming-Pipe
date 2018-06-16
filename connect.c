/* Ronald Cotton
 * Assignment 4 - connect
 * Processes - Signal and Pipes
 * CS360 - Systems Programming - Instructor: Dick Lang, Ph.D.
 * 
 * compiled with:
 * gcc -g -Wall -pedanic -ansi -std=c99
 * 		(will get warnings)
 * 
 * This program demonstrates the use of pipe, forks, signals and kills.
 */
 
/* includes */
#include <stdio.h>		/* for fprintf() */
#include <string.h>		/* for strcmp() */
#include <stdlib.h> 
#include <signal.h>		/* for SIGKILL */
#include <unistd.h>		/* for pipe(), dup(), execvp(), kill() */

/* defines 
 * MINARGS - This is the minimum number of arguments required for the 
 * program connect. */
#define MINARGS 4
/* STDIN, STDOUT - This is the fd for both, STDERR is not used */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

/* function definitions */
void usageConnect(char *command);
void errorOutput(int e);

/* main.c
 * This handles the arguments for connect, the usage only allows
 * two elements in the pipe. If one of the processes gets terminated or
 * is not valid, it will kill the child process (either the child kills
 */
int main(int argc, char* argv[]) {
	if (argc<MINARGS) usageConnect(argv[0]);
	
	int argOneIndex = 1, argTwoIndex;	/* index from argv[] */
	int colonCount = 0;					/* colonCount - program only allows 1 */
	
	for (int i=0; i<argc; ++i) {
		if ((strcmp(argv[i],":")) == 0) {
			if ((i==0)||(i==argc-1)) usageConnect(argv[0]); 
										/* ERROR - colon cannot be at beginning of the end */
			++colonCount;
			if (colonCount>1) usageConnect(argv[0]); 
										/* ERROR - cannot handle more than one pipe */
			argTwoIndex = i+1;			/* set the second command */
			argv[i]=NULL;				/* place a null for the first argument */
		} /* end if strcmp */
	} /* end for i */
	
	/* child_pid is the childs process id
	 * pfd is the pipes file descriptors
	 * returnpipe - used to check if the pipe opened correctly
	 * dupfd - dup returns the file descriptor that was returned
	 */ 
	int child_pid = 0, pfd[2], returnpipe, dupfd;
	
	returnpipe = pipe(pfd);				/* pfd[0] - prdr - read end of pipe */
										/* pfd[1] - pwtr - write end of pipe */
	int prdr = pfd[0], pwtr = pfd[1];
	
	if (returnpipe<0) errorOutput(1);
	
	/* ready to fork */
	if ((child_pid = fork())) {
		/* parent - parent runs second argument in the pipe */
		close(pwtr);		/* close the pipes writer for parent */
		close(STDIN);		/* close the stdin from parents FD */
		dupfd=dup(prdr);	/* replaces the FD of stdin with reader */
		close(prdr);		/* close the original reader fd */
		execvp(argv[argTwoIndex],&argv[argTwoIndex]);
		fprintf(stderr,"%s command failed.\n",argv[argTwoIndex]);
		kill(child_pid, SIGKILL);
		exit(1);
	} else {
		/* child - child run the first argument in the pipe */
		close(prdr);		/* close the pipes reader for child */
		close(STDOUT);		/* close the stdout from childs FD */
		dupfd=dup(pwtr);	/* replaces the FD of stdout with writer */
		close(pwtr);		/* close the original writer fd */
		execvp(argv[argOneIndex],&argv[argOneIndex]);
		fprintf(stderr,"%s command failed.\n",argv[argOneIndex]);
		kill(child_pid, SIGKILL);
		exit(1);
	} /* end if fork() */ 
	
return 0;
 } /* end main() */

/* usageConnect()
 * Displays at errors
 */
void usageConnect(char* command) {
	fprintf(stderr,"USAGE: %s <input stream pipe> : <output stream pipe>\n",command);
	fprintf(stderr,"\n\tYou may not place a pipe at the beginning or the end of the connect expression.");
	fprintf(stderr,"\n\tIf connect is unable to execute one side of the pipe, connect will 1 all processes.\n");
	exit(1);
} /* end usageConnect */

/* errorOutput()
 * If unable to create pipes, will print error, this code allows for more
 * error entries.
 */
void errorOutput(int e) {
	switch(e) {
		case 1: fprintf(stderr, "Unable to create pipe.\n"); break;
		default: break;
	} /* end switch() */

	exit(1);
} /* end errorOutput() */

