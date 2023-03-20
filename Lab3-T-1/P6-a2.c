/*
   ----- 48450 -- week 4 lab handout 1.2 ------
This is a program to use pipe to transfer the message from Thread A to Thread B.

Question: How this program can control the critical secion in memory?
*/
// gcc P6-a2.c -o P6.o -pthread && ./P6.o

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

typedef char buffer_item;
#define BUFFER_SIZE 100

buffer_item buffer[BUFFER_SIZE]; /* the buffer */

int fd[2];           // File descriptor for creating a pipe
pthread_t tid1;      // Thread ID
pthread_attr_t attr; // Set of thread attributes

/*This function continously reads fd[0] for any input data byte
If available, prints */

void *reader(void *param); // thread call read function
void *writer(void *param); // thread call write function
void initializeData();

int main()
{
  int result;
  printf("ok\n");

  void initializeData(); // run initialisation

  result = pipe(fd);
  if (result < 0)
  {
    perror("pipe error");
    exit(1);
  }

  /*create the thread 1*/
  if (pthread_create(&tid1, &attr, reader, NULL) != 0)
  {
    perror("failed create thread 1\n");
    exit(1);
  }

  /*wait for the thread to exit*/
  pthread_join(tid1, NULL);
  return 0;
}

void *reader(void *param)
{
  printf("In reading thread\n");

  char c[100];
  FILE *fptr;
  char file_name[9] = "data.txt";

  if ((fptr = fopen(file_name, "r")) == NULL)
  {
    printf("Error! opening file");
    // Program exits if file pointer returns NULL.
    exit(1);
  }

  // reads text until newline is encountered
  printf("reading from the file:\n");

  while (fgets(c, sizeof(c), fptr) != NULL)
  {
    fputs(c, stdout);
  }

  fclose(fptr);
}

// This function continously writes Alphabet into fd[1]
// Waits if no more space is available

void initializeData()
{
  /*get the default attributes*/
  pthread_attr_init(&attr);
}
