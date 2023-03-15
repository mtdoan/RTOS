/*
   ----- 48450 -- week 3 lab handout 4 ------

Let's consider the producer-consumer problem by creating two threads: the producer and the consumer.
To avoid the critical section problem, we use mutex to lock and unlock the program during the reading
and writing process.

-----------------------------------------
Question 1, see below the question is embedded in the code lines
Question 2, see below the question is embedded in the code lines
Question 3: If we comment the lines 75 & 76, what will be the results after runs.
Question 4: What is the IPC method used here (message passing or shared memory)?

Note: For Questions 1, 2 and 3 you need to use this original c code file for each question as above.
In other words, you need to answer each question by compared with the results run
by this original c code and the modified c code listed in the quesitions. Please copy the running
results of the programs from your screen to the submission. No results no score!

You don't need to change the code for Question 4. Please give your explanation for your choice.
*/

/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

typedef char buffer_item;
#define BUFFER_SIZE 100

#define RAND_DIVISOR 800000000

/************Global variables ******************/
pthread_mutex_t mutex; /* The mutex lock */

buffer_item buffer[BUFFER_SIZE]; /* the buffer */

int counter;     /* buffer counter This is the global variable might cause critical section*/
int Max_counter; /* hold the Max counter unmber for aligned printing as input*/

pthread_t tid1, tid2; // Thread ID
pthread_attr_t attr;  // Set of thread attributes

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param); /* the producer thread */
void *consumer(void *param); /* the consumer thread */
void initializeData();

/******************* Main*********************/
int main()
{

   /* Initialize the app */
   initializeData();
   /* Acquire a string from command line*/
   buffer_item inputs[100];
   printf("please input a string for BoundBuffer:\n");
   if (fgets(inputs, sizeof(inputs), stdin) == 0)
   {
      perror("failed fgets ]\n");
      exit(1);
   }

   /* Create the producer threads */
   printf("create the producer thread\n");
   pthread_create(&tid1, &attr, producer, inputs);

   sleep(1); /* to delay and wait for starting the procedure thread */

   /* Create the consumer threads */
   printf("create the consumer Thread\n");
   pthread_create(&tid2, &attr, consumer, NULL);

   /* If we comment the fllowing pthread_join, what will be*/
   pthread_join(tid1, NULL);
   pthread_join(tid2, NULL);
   sleep(1);
   /* Exit the program */
   printf("Exit the program\n");
   exit(0);
} /* end of main()*/

/***************** call functions from main ****************/
void initializeData()
{

   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);

   /* Get the default attributes */
   pthread_attr_init(&attr);

   /* init buffer, this is the golab varible which is the key element of this program for CRITICAL SECTION !!!*/
   counter = 0;
   Max_counter = 0;
} /* end of Init. */

/* Producer Thread */
void *producer(void *param)
{
   /* copy the input string into local variables*/
   buffer_item item[100] = {0};
   strcpy(item, (buffer_item *)param);

   /********************************************************
   Qeustion 1: if we comment pthread_mutex_lock and pthread_mutex_unlock in the following code lines. What will be the results? Give your explanations.
   *******************************************************/

   /* acquire the mutex lock */
   // pthread_mutex_lock(&mutex);
   printf("\nProducer mutex on, the producer prints the order as the user types on the keyboard \n");
   sleep(3); /* to delay and wait for starting the consumer thread */
   int i, k;
   i = k = 0;
   while (item[i] != '\0' && counter < BUFFER_SIZE) /* '\0' means the end of the buffer */
   {
      k = insert_item(item[i]);
      printf("%c", item[i]);
      i++;
   }

   printf("\nBuffer is loaded & producer mutex off\n");
   /* release the mutex lock */
   // pthread_mutex_unlock(&mutex);
   return 0;
} /* end of producer */

/* Consumer Thread */
void *consumer(void *param)
{
   buffer_item item = '0';

   /********************************************************
  Qeustion 2: To follow the Question 1, if we comment pthread_mutex_lock and pthread_mutex_unlock in the following code lines what will be the results? Give your explanations.
  *******************************************************/

   /* aquire the mutex lock */
   pthread_mutex_lock(&mutex);
   printf("\nconsumer mutex on, the consumer thread prints the revsersed order of the characters \n");
   Max_counter = counter; // save the Max number of input for the aligned printing
   while (counter)
   {
      remove_item(&item);
      printf("%c", item);
   }
   printf("\nEmpty & consumer mutex off\n");
   /* release the mutex lock */
   pthread_mutex_unlock(&mutex);
   return 0;
} /* end of comsumer */

/*********************** subroutines ************************/

/* Add an item to the buffer */
int insert_item(buffer_item item)
{
   /* When the buffer is not full add the item
      and increment the counter*/
   if (counter < BUFFER_SIZE)
   {
      buffer[counter] = item;
      counter++;
      return 0;
   }
   else
   { /* Error the buffer is full */
      return -1;
   }
}

/* Remove an item from the buffer */
int remove_item(buffer_item *item)
{
   /* When the buffer is not empty remove the item
      and decrement the counter */

   if (counter > 0)
   {
      *item = buffer[(Max_counter - counter - 1)];
      counter--;
      return 0;
   }
   else
   { /* Error buffer empty */
      return -1;
   }
}

// gcc P4-BoundBuffer_multithread.c -o P4-BoundBuffer_multithread.out && ./P4-BoundBuffer_multithread.out


// Q3: consumer Thread code cannot run because it was not created.
