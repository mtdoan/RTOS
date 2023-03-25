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
#include <fcntl.h>

typedef char buffer_item;
#define BUFFER_SIZE 100

buffer_item buffer[BUFFER_SIZE]; /* the buffer */

sem_t semaphore_b; /* the semaphores */
int fd[2];         // File descriptor for creating a pipe
int fd_fifo;
pthread_t tid_a, tid_b; // Thread ID
pthread_attr_t attr;    // Set of thread attributes

/*This function continously reads fd[0] for any input data byte
If available, prints */

void *write_to_pipe(void *param); // thread call write_to_pipe function
void *read_from_pipe(void *param);
void *writer(void *param); // thread call write function
void initializeData();

int main()
{
  int result;
  printf("Start Assignment 2\n");

  void initializeData(); // run initialisation

  result = pipe(fd);
  if (result < 0)
  {
    perror("An error ocurred with opening the pipe");
    exit(1);
  }

  /*create the thread A*/
  if (pthread_create(&tid_a, &attr, write_to_pipe, NULL) != 0)
  {
    perror("failed create thread 1\n");
    exit(1);
  }

  /*create the thread B*/
  if (pthread_create(&tid_b, &attr, read_from_pipe, NULL) != 0)
  {
    perror("failed create thread 1\n");
    exit(1);
  }
  /*wait for the thread to exit*/
  pthread_join(tid_a, NULL);
  pthread_join(tid_b, NULL);

  return 0;
}

// thread A writes data
void *write_to_pipe(void *param)
{
  printf("In writing thread A\n");

  char c[100];
  FILE *fptr;
  char file_name[9] = "data.txt";
  fptr = fopen(file_name, "r");
  if (fptr == NULL)
  {
    perror("An error ocurred with opening input file");
    exit(1);
  }

  printf("reading from the file data.txt:\n");
  // reads text until newline is encountered
  while (fgets(c, sizeof(c), fptr) != NULL)
  {
    int len = strlen(c);
    int written_bytes = write(fd[1], c, len);
    if (written_bytes != len)
    {
      perror("An error ocurred with writting data into the pipe");
      exit(2);
    }

    printf("%s", c);
  }
  printf("End reading file in thread A\n");
  fclose(fptr);
  close(fd[1]);
  sem_post(&semaphore_b);
}

// thread B reads data
void *read_from_pipe(void *param)
{
  sem_wait(&semaphore_b);
  printf("______________________\n\n");
  printf("In reading thread B\n");
  char ch[1];
  size_t len = 0;
  size_t cap = 100;
  char *string_to_read;
  string_to_read = malloc(cap * sizeof(char));
  int reading_result;

  while (1)
  {
    reading_result = read(fd[0], ch, 1);
    if (reading_result == 0)
    {
      string_to_read[len] = '\0';
      printf("%s\n", string_to_read);
      printf("Reading pipe has completed in thread B\n");
      close(fd[0]);
      free(string_to_read);
      exit(0);
    }
    else if (reading_result == -1)
    {
      perror("An error ocurred with reading data from the pipe");
      exit(1);
    }
    string_to_read[len] = ch[0];
    len++;
    if (len == cap)
    {
      cap *= 2;
      string_to_read = realloc(string_to_read, cap * sizeof(char));
    }
    if (!string_to_read)
    {
      fprintf(stderr, "%s\n", "Error while allocating memory\n");
      exit(1);
    }
  }
}

void initializeData()
{
  if (sem_init(&semaphore_b, 0, 0) != 0)
  {
    printf("\nsem_b has failed\n");
    exit(1);
  }

  /*get the default attributes*/
  pthread_attr_init(&attr);
}
