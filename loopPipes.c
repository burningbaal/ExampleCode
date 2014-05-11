//Keith McKinley
//mckinleykj@gmail.com

/*	Example of a way to loop a set of pipes that simply pass a string through the layers, 
|	then _exit()'s it's way back out.  Think of it as the film 'Inception'.
|	This was written entirely by me, during a class in Operating Systems at Oregon State 
|	I wrote it to ensure that I knew how pipes were working before working on a homework assignment
*/
#include<stdio.h>
#include<unistd.h>
//#include<string.h>
#include<stdlib.h>
#include<errno.h>
//#include <time.h>
//#include <ctype.h>
//#include <fcntl.h>
//#include <dirent.h>

#define MAX_DEPTH 5
#define BUF_SIZE 512
#define testString "Hello World!"

int main(int argc, char **argv) {
	int i, j, k;
	int fds[MAX_DEPTH][2];
	int currentDepth=0;
	int fds1[2];
	int fds2[2];
	int childId[MAX_DEPTH];
	char buf[BUF_SIZE];
	int *status;
	int numRead;
	for (currentDepth=0;currentDepth < MAX_DEPTH; currentDepth ++) {
		if (-1 == pipe(fds[currentDepth])) {
			perror("Error creating pipes");
			exit(EXIT_FAILURE);
		}
		switch(childId[currentDepth] = fork()) {
			case 0:
				//child
				close(fds[currentDepth][1]);
				//printf("in %dth child case, pid = %d\n", currentDepth, getpid());
				if (currentDepth == MAX_DEPTH-1) {
					numRead = read(fds[currentDepth][0], buf, BUF_SIZE);
					buf[numRead] = '\0';
					printf("%s is printed by the %d layer, pid= %d\n", buf, currentDepth, getpid());
					_exit(EXIT_SUCCESS);
				} else {
					//printf("in %dth child case's 'else', pid = %d\n", currentDepth, getpid());
					numRead = read(fds[currentDepth][0], buf, BUF_SIZE);
					buf[numRead] = '\0';
					printf("\t%dth child has: %s, pid= %d\n", currentDepth, buf, getpid());
				}
				break;
			default:
				//parent
				close(fds[currentDepth][0]);
				//printf("in parent case, pid = %d\n", getpid());
				if (currentDepth == 0) {
					write(fds[currentDepth][1], testString, sizeof(testString));
				} else {
					//printf("%d parent has: %s\n", currentDepth, buf);
					write(fds[currentDepth][1], buf, numRead+1);
				}
				//printf("in %dth parent, post-write, my pid = %d\n", currentDepth, getpid());
				close(fds[currentDepth][1]);
				waitpid(childId[currentDepth], status, WUNTRACED);
				printf("   Finished waiting for %dth child (%d), my pid= %d\n", currentDepth, childId[currentDepth], getpid() );
				_exit(EXIT_SUCCESS);
				break;
		}
	}
	return 0;
}