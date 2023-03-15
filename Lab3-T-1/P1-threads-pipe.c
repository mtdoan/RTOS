/*
   ----- 48450 -- week 4 lab handout 1.2 ------
This is a program to use pipe to transfer the message from Thread A to Thread B.

Question: How this program can control the critical secion in memory?
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

typedef char buffer_item;
#define BUFFER_SIZE 100

buffer_item buffer[BUFFER_SIZE]; /* the buffer */

int fd[2];            // File descriptor for creating a pipe
pthread_t tid1, tid2; // Thread ID
pthread_attr_t attr;  // Set of thread attributes

/*This function continously reads fd[0] for any input data byte
If available, prints */

void *reader(void *param); // thread call read function
void *writer(void *param); // thread call write function
void initializeData();

int main()
{
   int result;
   buffer_item inputs[100];

   printf("please input a string for this A thread ->B thread:\n");
   if (fgets(inputs, sizeof(inputs), stdin) == 0)
   {
      perror("failed of fgets\n");
      exit(1);
   }

   void initialiszeData(); // run initialisation

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

   /*create the thread 2*/
   if (pthread_create(&tid2, &attr, writer, inputs) != 0)
   {
      perror("failed create thread 2\n");
      exit(1);
   }

   /*wait for the thread to exit*/
   pthread_join(tid1, NULL);
   pthread_join(tid2, NULL);
}

void *reader(void *param)
{
   printf("In reading thread\n");
   while (1)
   {
      char ch;
      int result;

      result = read(fd[0], &ch, 1);
      if (result != 1)
      {
         perror("read");
         exit(4);
      }

      if (ch != '\0')
      {
         printf("Reader: %c\n", ch);
      }
      else
      {
         printf("reading pipe has completed\n");
         exit(5);
      }
   }
}

// This function continously writes Alphabet into fd[1]
// Waits if no more space is available

void *writer(void *param)
{
   int i = 0;
   int result;
   buffer_item item[100];

   printf("In writing thread\n");

   /* copy the input string into local variables*/
   strcpy(item, (buffer_item *)param);

   while (item[i] != '\0')
   {
      result = write(fd[1], &item[i], 1);
      if (result != 1)
      {
         perror("write");
         exit(2);
      }

      printf("%c", item[i]);
      i++;
   }

   /* add the '\0' in the end of the pipe */
   result = write(fd[1], &item[i], 1);
   if (result != 1)
   {
      perror("write");
      exit(3);
   }

   printf("\nwriting pipe has finished\n");
   return 0;
}

void initialiszeData()
{

   /*get the default attributes*/
   pthread_attr_init(&attr);
}

// gcc P1-threads-pipe.c -o P1a -pthread && ./P1a
