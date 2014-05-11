//Keith McKinley
//mckinlek@onid.oregonstate.edu
//CS311-400
//Homework #4
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>

static void sigIntHandler(int sig){
	printf("SIGINT has been caught, terminating the program\n");
	kill(getpid(), -9);
}

static void sigUsr1Handler(int sig){
	printf("SIGUSR1 has been caught\n");
	return;
}


static void sigUsr2Handler(int sig){
	printf("SIGUSR2 has been caught\n");
	return;
}

int main(int argc, char **argv) {
	struct sigaction sigInt, sigUsr1, sigUsr2, oldInt, oldUsr1, oldUsr2;
	
	//setup
	sigInt.sa_handler = sigIntHandler;
	sigInt.sa_flags = 0;
	sigUsr1.sa_handler = sigUsr1Handler;
	sigUsr1.sa_flags = 0;
	sigUsr2.sa_handler = sigUsr2Handler;
	sigUsr2.sa_flags = 0;
	
	//define handlers
	sigaction (SIGINT, NULL, &oldInt);
	if (oldInt.sa_handler != SIG_IGN)
		sigaction (SIGINT, &sigInt, NULL);
	sigaction (SIGUSR1, NULL, &oldUsr1);
	if (oldUsr1.sa_handler != SIG_IGN)
		sigaction (SIGUSR1, &sigUsr1, NULL);
	sigaction (SIGINT, NULL, &oldUsr2);
	if (oldUsr2.sa_handler != SIG_IGN)
		sigaction (SIGUSR2, &sigUsr2, NULL);
		
	//do it!
	kill(getpid(), SIGUSR1);
	kill(getpid(), SIGUSR2);
	kill(getpid(), SIGINT);
	
	
	return 0;
}
//end