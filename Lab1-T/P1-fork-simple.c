/*
	 ----- 48450 -- week 2 lab handout 1 ------

fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process.

On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, in which case, no child process is created, and errno is set appropriately.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void do_child_things()
{
	printf("This is a child process %d\n", (int)getpid());
	printf("Do child things\n");
}

void do_parent_things()
{
	printf("This is a parent process\n");
	printf("Do parent things\n");
}

int main()
{
	pid_t pid = fork();
	switch (pid)
	{
	case -1:
		perror("fork: error\n");
		exit(1);
	case 0:
		do_child_things();
		break;
	default:
		printf("child pid=%d, parent's=%d\n", (int)pid, (int)getpid());
		wait(NULL); // if comment this line, the parent exit without waiting child process
		do_parent_things();
		break;
	}
	return (0);
}

// gcc P1-fork-simple.c -o P1-fork-simple.out && ./P1-fork-simple.out
