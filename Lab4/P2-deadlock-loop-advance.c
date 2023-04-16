/*
----- 48450 -- week 5 lab handout 2 ------ 
By programing the handout 2, you will follow the lecture notes to implement the deadlock detection algorithm with the loop search method. 

This program code is for detecting the deadlock process

There are 5 process with 3 resouces
*/
 
#include<stdio.h>
int Request[5][3]; //the process request
int Allocation[5][3];// already allocate resources to process
int Available[3];//available instances for each resource
int p,r;//the number of process and resource

void input_data();
void cal();

/* ************************* Main ************************* */
int main()
{
    printf("\n********** Deadlock Detection Algo ************\n");
    input_data();
    cal();//detect deadlock
    return 0;
} /* end of Main */

/******************* Call functions ******************/
//call function-1: input example
void input_data()
{
    
    p=5;//Process No.
    r=3;//Resource no.
    //Enter the Request Matrix
    Request[0][0]=0;Request[0][1]=0;Request[0][2]=0;
    Request[1][0]=2;Request[1][1]=0;Request[1][2]=2;
    Request[2][0]=0;Request[2][1]=0;Request[2][2]=0;
    Request[3][0]=1;Request[3][1]=0;Request[3][2]=0;
    Request[4][0]=0;Request[4][1]=0;Request[4][2]=2;

/* Request matrix:
		A	B	C
		-----------------
		0	0	0
		2	0	2
		0	0	0
		1	0	0
		0	0	2
*/

    //Enter the Allocation Matrix
    Allocation[0][0]=0;Allocation[0][1]=1;Allocation[0][2]=0;
    Allocation[1][0]=2;Allocation[1][1]=0;Allocation[1][2]=0;
    Allocation[2][0]=3;Allocation[2][1]=0;Allocation[2][2]=3;
    Allocation[3][0]=2;Allocation[3][1]=1;Allocation[3][2]=1;
    Allocation[4][0]=0;Allocation[4][1]=0;Allocation[4][2]=2;

/* Allocation matrix:

		A	B	C
		-----------------
		0	1	0
		2	0	0
		3	0	3
		2	1	1
		0	0	2
*/

    //Enter the available Resources
    Available[0]=0;Available[1]=0;Available[2]=0;
/* Available Matrix:
		A	B	C
		-----------------
		0	0	0
*/
}


//call function-2: calculate dead-lock
void cal()
{
    int finish[5];//store whether the process finish and dead process id.
    int k,i,j,flag;//k,i,j is index variable. flag use for whether there is deadlock
    int runOrder[5]; //the process sequence order
    for(i=0;i<p;i++)
    {
         finish[i]=0; // all initially set up False = 0
    }

/* -----------detect deadlock process------------ */
    flag=0;//whether there is a deadlock: 0 is no deadlock, 1 is deadlock
    int pro_id=0;
    	
    while((flag==0) && pro_id<p)
    {
	int possbileLock=1;
	for(i=0;i<p;i++)// for every process, to check whether it is OK to run
	{	
	  if (finish[i]==0){ //go to check. Otherwise, this process has been passed the check 
	printf("\nstart_a_new_process_ID=%d\n", i);
		
	   int c=0;// temporal variable for resource counting 
	   for(j=0;j<r;j++)//for every resoure. total is 3
	   {
/* Question-1: Why do we need the code line --'Request[i][j]<=Available[j])'?  */ 
	printf("current process i=%d, current resource j=%d, c=%d\n", i,j, c);
	if(Request[i][j]<=Available[j] && finish[i]==0) //check point
	  {
		c++;
/* Question-2 What do the following code lins 78 to 86 acheive? */
	    if(c==r) { 
		flag=0; 

		for(k=0;k<r;k++)
		Available[k]+=Allocation[i][k];
		 
		possbileLock=0;
		finish[i]=1;
		runOrder[pro_id]=i;//record the current finish process id
		}
	   }
	   else// if check point fails, there is one resource not satisfy the request, it might be deadlock
	   {
	possbileLock=1;
	printf("possible_deadlock_process_ID = %d and failed this time !\n", i);
	break;
	  }	 
	}// for j end
	/*if there is one process can run, go out and run the process*/
	    if(possbileLock==0){ 
		printf("run the process(runOrder[%d])\n", i);
		//run the process(runOrder[pro_id]);		
		break;}  	
      }
   }//for i end
	
   if(i==p && possbileLock==1)//there is a deadlock indeed!
	flag=1; 

	pro_id++; // So far so good for the process with deadlock. 
     }//while end
     

/* --------------display deadlock process----------------- */
     if(flag==1)
     {
         printf("\nSystem is in Deadlock!\n");
	 // output the deadlock processes
	 printf("****************************************\n");
	 printf("Deadlock processes are: ");
	 for(i=0;i<p;i++)
	 {
		if(finish[i]==0)
		    printf("P%d ",i+1);
	 }
	 printf("\n****************************************\n\n");
     }
     else
     {   
	 printf("\nHappy finish. No Deadlock Occur!\n");
	 printf("****************************************\n");
	 printf("The running sequence is:  ");
	 for(i=0;i<p;i++)
	 {
	    printf("P%d",runOrder[i]);
	    if(i<p-1)
		printf("->");
	 }
	 printf("\n****************************************\n\n");
         
     }  
}//Cal end

