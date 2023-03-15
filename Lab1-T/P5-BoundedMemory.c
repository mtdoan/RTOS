/*
   ----- 48450 -- week 2 lab handout 5------ 

To illustrate the concept of cooperating processes -- IPC, 

let us consider the producer-consumer problem, which is a common paradigm for cooperating processes. A producer process produces information that is consumed by a consumer process

Method 1 -- shared memory
*/

/**********************************************/
/*	Question 1  -- Draw a flow chat of this program including the producer and consumer process. You can refer to the lecture notes in UTSOnline 
	Question 2 -- which variable declaration is used for shared memory?
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define BUFFER_SIZE 15 /* space from 0-14 */
#define TRUE 1

char *text="Jian Zhang OS Code";/*  17 characters*/

typedef struct
{
  char elem;
}item;

int main()
{
  item buffer[BUFFER_SIZE];/* buffer size is declared as 15, buffer [0] to buffer [14]*/
  int in=0;
  int out=0;

  item next_produced, next_comsumed;
  while(TRUE)
  {
	printf("Begin produce:\n");
	while(1)
	{
	    if(((in+1)%BUFFER_SIZE)==out) /* module 15*/
		{
		printf("\ncharacter %d (15 characters)\nThe proceduce is full\n", in);
		break;
		}//break and enter the consume process
 
	    next_produced.elem=text[in]; // assign one character (text) to next_produced.elem
	    buffer[in]=next_produced;
	    printf("%c, character %d\n", next_produced.elem, in); 
 	  
	    in=in+1;
	}

	printf("\nBegin comsume:\n");
	while(TRUE)
	{
	    if(in==out)
	    {
		printf("\nComsume complete, it is empty now.\n");break;
	    }//break and exit this while loop

	    next_comsumed=buffer[out];
  	    printf("%c",next_comsumed.elem);
	    out=(out+1)%BUFFER_SIZE; //comsume one character from the next_comsumed.elem
	}
	

	break;
  }// complete the process and end
  
  
}


