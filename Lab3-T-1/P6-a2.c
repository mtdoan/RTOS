/*
   ----- 48450 -- week 4 lab handout 1.2 ------
This is a program to use pipe to transfer the message from Thread A to Thread B.

Question: How this program can control the critical secion in memory?/au/courses/9615/workspaces
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

sem_t semaphore_2, semaphore_3; /* the semaphores */
int fd[2];         // File descriptor for creating a pipe
int fd_bc[2];
pthread_t tid_1, tid_2, tid_3; // Thread ID
pthread_attr_t attr;    // Set of thread attributes

/*This function continously reads fd[0] for any input data byte
If available, prints */

void *write_to_pipe(void *param); // thread call write_to_pipe function
void *read_from_pipe(void *param);
void *read_from_pipe_bc(void *param);

void *writer(void *param); // thread call write function
void initializeData();

int main()
{
  int result;
  int result_bc;
  printf("Start Assignment 2\n");

  void initializeData(); // run initialisation

  result = pipe(fd);
  result_bc = pipe(fd_bc);

  if (result < 0 || result_bc < 0)
  {
    perror("An error ocurred with opening the pipe");
    exit(1);
  }

  /*create the thread 1*/
  if (pthread_create(&tid_1, &attr, write_to_pipe, NULL) != 0)
  {
    perror("failed create thread 1\n");
    exit(1);
  }

  /*create the thread 2*/
  if (pthread_create(&tid_2, &attr, read_from_pipe, NULL) != 0)
  {
    perror("failed create thread 2\n");
    exit(1);
  }
  
  /*create the thread 3*/
  if (pthread_create(&tid_3, &attr, read_from_pipe_bc, NULL) != 0)
  {
    perror("failed create thread 3\n");
    exit(1);
  }

  /*wait for the thread to exit*/
  pthread_join(tid_1, NULL);
  pthread_join(tid_2, NULL);
  pthread_join(tid_3, NULL);

  return 0;
}

// thread 1 writes data
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
  printf("Writing data to pipe AB has completed in thread A\n");
  fclose(fptr);
  close(fd[1]);
  sem_post(&semaphore_2);
}

// thread 2 reads data
void *read_from_pipe(void *param)
{
  sem_wait(&semaphore_2);
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
      printf("Reading pipe AB has completed in thread B\n");
      close(fd[0]);
      //
      int written_bytes = write(fd_bc[1], string_to_read, len);
      if (written_bytes != len)
      {
        perror("An error ocurred with writting data into the pipe");
        exit(2);
      } else {
        printf("Writing data to pipe BC has completed in thread B\n");
        sem_post(&semaphore_3);
      }
      //
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

// thread 3 reads data from pipe BC
void *read_from_pipe_bc(void *param)
{
  sem_wait(&semaphore_3);
  printf("______________________\n\n");
  printf("Invoke thread 3\n");
  printf("Invoke thread 31\n");
  printf("Invoke thread 32\n");
  fflush(stdout);
  exit(0);
}

void initializeData()
{
  if (sem_init(&semaphore_2, 0, 0) != 0)
  {
    printf("semaphore_2 has failed\n");
    exit(1);
  }
  if (sem_init(&semaphore_3, 0, 0) != 0)
  {
    printf("semaphore_3 has failed\n");
    exit(1);
  }

  /*get the default attributes*/
  pthread_attr_init(&attr);
}
