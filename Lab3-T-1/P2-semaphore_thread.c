 /*
   ----- 48450 -- week 4 lab handout 2 ------ 
By programing the handout 2, you can try to add the semaphore to control the critical section 
of two threads' operation: if the runnerOne is running, the runnerTwo should wait. 
Question-1 How to control the run order of 'runnerOne' and 'RunnerTwo'?
Question-2 If we comment lines 49 and 50, what is the result?
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int sum;/*global variable. this variable is shared by the threads*/

/* the semaphores */
sem_t semaphore_one, semaphore_two;
pthread_t tid1,tid2;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *runnerOne(void *param);/*thread call this function*/
void *runnerTwo(void *param);/*thread call this function*/
void initializeData();

int main(int argc, char*argv[])
{
  int flag; /* the flag to set the sem_post to lead the runnerOne or runnerTwo thread */

  if(argc!=2){
	fprintf(stderr,"usage: ./P2 number  \n<note: an integer value for loop counting times & the even value will set runnerOne go first and vice versa for the odd value>\n");
	return -1;
  }
  if(atoi(argv[1])<0){
	fprintf(stderr,"%d must be >=0\n",atoi(argv[1]));
	return -1;
  }
  initializeData();

  printf("Initial sum=%d\n",sum);

  /*create the thread 1*/
  if (pthread_create(&tid1, &attr, runnerOne, argv[1]) !=0) {
	printf ("\n Thread-1 can't be created \n");exit (1);}

  
  /*create the thread 2*/
  if (pthread_create(&tid2, &attr, runnerTwo, argv[1]) !=0) {
	printf ("\n Thread-2 can't be created \n");exit (1);}


  flag=atoi(argv[1])%2; /* calcuate the modulo % of 2, if it is even number T1 goes first otherwise, T2 goes first! */
  switch (flag){
	case 0: sem_post(&semaphore_one); break; //unlock semaphore_one
	case 1: sem_post(&semaphore_two); break; //unlock semaphore_two
	default: printf("Invalid swtich\n" );
  }

	  	 
  /*wait for the thread to exit*/
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
 	

  /* why sum values are different if we set 'one' first and vice versa ? */  
  printf("sum=%d\n",sum);

}

/*The thread will begin control in this function*/
void *runnerOne(void *param)
{
  /* waitijng for aquiring the full lock */
  sem_wait(&semaphore_one);

  int i,upper=atoi(param);
  
  printf("thread one, the first value of sum=%d\n", sum);
  for(i=0;i<=upper;i++)
    sum=sum+2*i;

  printf("thread one, the final valeu of sum=%d\n", sum);

  /* signal empty */
  sem_post(&semaphore_two);
return 0;
}

/*The thread will begin control in this function*/
void *runnerTwo(void *param)
{
  /* waiting to aquire the full lock */
  sem_wait(&semaphore_two);

  int i,upper=atoi(param);
  printf("thread two, the first value of sum=%d\n",sum);
  for(i=0;i<=upper;i++)
    sum=sum+i;
  printf("thread two, the final value of sum=%d\n",sum);

  /* signal empty */
  sem_post(&semaphore_one);
  return 0;
}

void initializeData() {
   sum=0;

   /* Initialize the named semaphore_one, 0-shared between threads, initial value is set to one=0 */
   if(sem_init(&semaphore_one, 0, 0)!=0) {
	printf("\nsem_one has failed\n"); exit(1);}

   /* Initialize the named semaphore_two, 0-shared between threads, initial value is set to two=0 */
   if(sem_init(&semaphore_two, 0, 0)!=0) {
	printf("\nsem_two has failed\n"); exit(1);}

   /* Get the default attributes */
   if (pthread_attr_init(&attr)!=0) {
	printf("\n pthread_attr_init has failed\n"); exit(1);}	
}

// gcc P2-semaphore_thread.c -o P2a -pthread && ./P2a
