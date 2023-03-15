/*
   ----- 48450 -- week 4 lab handout 1 ------
This is a simple program to use pipe to transfer the message from parent process to child process.

Question: How this program can control the critical secion in memory?
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MESSLENGTH 80

char pourIntoPipe[] = "This has been through the pipe!\n";
char collectFromPipe[MESSLENGTH];

int main(void)
{
  int fd[2];
  int n;
  pid_t pid;

  if (pipe(fd) < 0) /* create a pipe: fd[0] is a pointer for reading and fd[1] for writing */
    perror("pipe error");
  if ((pid = fork()) < 0)
    perror("fork error");
  else if (pid > 0)
  {               /* parent will do the writing this time */
    close(fd[0]); /* we don't want the reading end at all */
    n = write(fd[1], pourIntoPipe, strlen(pourIntoPipe));
    if (n == 0)
    {
      perror("failed writing\n");
      exit(1);
    }

    printf("Parent is writing to the pipe: %s", pourIntoPipe);
    wait(NULL);
  }
  else
  {
    close(fd[1]);

    /* child will do the reading. */
    n = read(fd[0], collectFromPipe, MESSLENGTH);
    if (n == 0)
    {
      perror("failed writing\n");
      exit(1);
    }

    printf("Child is reading from the pipe: %s", collectFromPipe);
  }
  return (0);
}

// gcc P1-simple_pipe.c -o P1 -pthread && ./P1
