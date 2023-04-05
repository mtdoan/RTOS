//***********************************************************************************
// Author: Minh Thuy Doan
//***********************************************************************************
/*
  To compile prog_1 ensure that gcc is installed and run the following command:
  gcc prog_1.c -o prog_1 -lpthread -lrt -Wall
  gcc assign2_template-v3.c -o a2.o -lpthread -lrt -Wall && ./a2.o
*/

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/time.h>

/* --- Defines --- */
#define BUFFER_SIZE 1000

/* --- Structs --- */
typedef struct ThreadParams
{
  int pipeFile[2];
  sem_t sem_A, sem_B, sem_C; /* the semaphores */
  char message[BUFFER_SIZE];
} ThreadParams;

pthread_attr_t attr; // Set of thread attributes

/* --- Shared memory --- */
volatile int message_len = 0;

/* --- Prototypes --- */

/* Initializes data and utilities used in thread params */
void initializeData(ThreadParams *params);

/* This thread reads data from data.txt and writes each line to a pipe */
void *ThreadA(void *params);

/* This thread reads data from pipe used in ThreadA and writes it to a shared variable */
void *ThreadB(void *params);

/* This thread reads from shared variable and outputs non-header text to output.txt */
void *ThreadC(void *params);

/* --- Main Code --- */
int main(int argc, char const *argv[])
{

  pthread_t tid[3];
  ThreadParams params;

  printf("Start Assignment 2\n");

  // Initialization
  initializeData(&params);

  // init pipe
  if (pipe(params.pipeFile) < 0)
  {
    perror("An error ocurred with opening the pipe");
    exit(1);
  }

  // Create Threads
  if (pthread_create(&(tid[0]), &attr, &ThreadA, (void *)(&params)) != 0)
  {
    perror("failed to create thread A\n");
    exit(1);
  }

  if (pthread_create(&(tid[1]), &attr, &ThreadB, (void *)(&params)) != 0)
  {
    perror("failed to create thread B\n");
    exit(1);
  }

  if (pthread_create(&(tid[2]), &attr, &ThreadC, (void *)(&params)) != 0)
  {
    perror("failed to create thread C\n");
    exit(1);
  }

  // Wait on threads to finish
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);

  return 0;
}

void initializeData(ThreadParams *params)
{
  // Initialize Sempahores
  sem_init(&(params->sem_A), 0, 1);
  sem_init(&(params->sem_B), 0, 0);
  sem_init(&(params->sem_C), 0, 0);

  // Initialize thread attributes, with the default attributes
  pthread_attr_init(&attr);

  return;
}

void *ThreadA(void *params)
{
  printf("\nThread A reads from data.txt\n");
  ThreadParams *TP = params;
  FILE *reader;
  char file_name[9] = "data.txt";
  reader = fopen(file_name, "r");
  if (reader == NULL)
  {
    perror("An error ocurred with opening input file");
    exit(1);
  }
  char string_to_read[BUFFER_SIZE];
  // reads text until newline is encountered
  while (fgets(string_to_read, BUFFER_SIZE, reader) != NULL)
  {
    int len = strlen(string_to_read);
    int written_bytes = write(TP->pipeFile[1], string_to_read, len);
    if (written_bytes != len)
    {
      perror("An error ocurred with writting data into the pipe");
      exit(2);
    }
    printf("Thread A - written_bytes: %s", string_to_read);
  }
  printf("Data is written to pipe from thread A\n");
  fclose(reader);
  close(TP->pipeFile[1]);
  sem_post(&(TP->sem_B));
  pthread_exit(0);
}

void *ThreadB(void *params)
{
  ThreadParams *TP = params;
  sem_wait(&(TP->sem_B));
  printf("\nThread B reads data from pipe used in ThreadA and writes it to a shared variable\n");
  message_len = read(TP->pipeFile[0], TP->message, BUFFER_SIZE);
  TP->message[message_len + 1] = '\0';
  printf("Thread B - message:\n%s", TP->message);
  close(TP->pipeFile[0]);
  sem_post(&(TP->sem_C));
  pthread_exit(0);
}

void *ThreadC(void *params)
{
  ThreadParams *TP = params;
  sem_wait(&(TP->sem_C));
  printf("\nThread C reads from shared variable and outputs non-header text to output.txt\n");
  char *check = "end_header\n";
  int sig = 0;

  char *string_to_write = malloc(BUFFER_SIZE * sizeof(char));
  FILE *writer;
  writer = fopen("output.txt", "w");
  if (writer == NULL)
  {
    perror("An error ocurred with openning output.txt file");
    exit(1);
  }
  int index = 0;
  size_t len = 0;

  for (index = 0; index <= message_len + 1; index++)
  {
    string_to_write[len] = TP->message[index];
    len++;

    // process each line
    if (string_to_write[len - 1] == '\n')
    {
      string_to_write[len] = '\0';
      if (sig == 1)
      {
        fputs(string_to_write, writer);
        printf("Thread C - string_to_write: %s", string_to_write);
      }
      // compare with check value, switch sig flag
      if ((sig == 0) && strcmp(string_to_write, check) == 0)
      {
        // The end of header
        sig = 1;
      }
      // reset the len to read new line
      len = 0;
    }
  }
  free(string_to_write);
  fclose(writer);
  pthread_exit(0);
}
