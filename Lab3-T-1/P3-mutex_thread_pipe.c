/*
   ----- 48450 -- week 4 lab handout 3 ------ 
By programing the handout 3, you can try to add the mutex and threads to control the critical section of two threads' operation: if the runnerOne is running, the runnerTwo should wait. 

Qeustion-1, why we need to add the sleep (1) in line 48?
Question-2, can we comment the lines 55 and 56?
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


/* The mutex lock */
pthread_mutex_t mutex;
pthread_t tid1, tid2; //Thread ID
pthread_attr_t attr; //Set of thread attributes

#define MESSLENGTH 80

char indicate[] = "This has been through the pipe!\n";
char pourIntoPipe[] = " ";

char collectFromPipe[MESSLENGTH];
void *runnerOne(void *param);/*threads call this function*/
void *runnerTwo(void *param);/*threads call this function*/
void initializeData();

int main(int argc, char*argv[])
{
	
if (argc != 2) {
		fprintf(stderr, "USAGE:./main.out input 'characters'\n");	
	}

if(atoi(argv[1])<0){
	fprintf(stderr,"%d must be >=0\n",atoi(argv[1]));
	return -1;
  }
	//convert the input strings into array of char
	strcpy(pourIntoPipe, argv[1]);

	// used to store pipe pointer
	int  fd[2];

	initializeData();

	if (pipe(fd) < 0)
		perror("pipe error");
   
		/*create the thread 1*/
	if (pthread_create(&tid1, &attr, runnerOne, fd) !=0) {
	printf ("\n Thread-1 can't be created \n");exit (1);}

	sleep (1);

	/*create the thread 2*/
	if (pthread_create(&tid2, &attr, runnerTwo, fd) !=0) {
	printf ("\n Thread-2 can't be created \n");exit (1);}

	/*wait for the thread to exit*/
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
 	pthread_mutex_destroy(&mutex);

	close(fd[0]);
	close(fd[1]);

}

/*The thread will begin control in this function*/
void *runnerOne(void *param)
{
	int i=0;
	int *fd = (int*)param;
	/* aquire the mutex lock */
	pthread_mutex_lock(&mutex);

	// write content to pipe
	i=write(fd[1], pourIntoPipe, strlen(pourIntoPipe));
	if(i==0){perror("failed write\n");exit(1);}
	printf("thread one: write to pipe\n");
	/* release the mutex lock */
	pthread_mutex_unlock(&mutex);
	return 0;
}

/*The thread will begin control in this function*/
void *runnerTwo(void *param)
{
	int i=0;
	int *fd = (int*)param;
	/* aquire the mutex lock */
	pthread_mutex_lock(&mutex);

	// read content from the pipe
	i=read(fd[0], collectFromPipe, MESSLENGTH);
	if(i==0){perror("failed read\n");exit(1);}
	printf("%s", collectFromPipe);
	printf("\n");

	printf("thread two: read from pipe\n");
	/* release the mutex lock */
	pthread_mutex_unlock(&mutex);
	return 0;
}

void initializeData() {
	

	/* Create the mutex lock */
pthread_mutex_init(&mutex, NULL);
	/* if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init has failed\n");
        exit(1);}*/

	/*get the default attributes*/
pthread_attr_init(&attr);
	
	/*if (pthread_attr_init(&attr)!=0) {
	printf("\n thread inti has failed\n");
        exit(1);}*/
	}
