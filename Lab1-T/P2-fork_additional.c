/*
   ----- 48450 -- week 2 lab handout 2 ------ 

fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process.

On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, in which case, no child process is created, and errno is set appropriately.
*/

/**********************************************/
/*	Question 1  -- Draw a flow chat of this program including the parant process and child process. You can refer to the lecture notes in UTSOnline 
	Questions 2, 3, 4 and 5 are embedded in the code lines
*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  pid_t pid; 

	
	/* fork a child process */
	pid=fork();
	
	printf("pid order: pid=%d\n",pid);//to see the pid execute order

	if(pid<0){/* error occurred*/
	  fprintf(stderr, "Fork Failed");
	  return 1;
	}
	else if(pid ==0){/* child process*/
	  printf("Child Begins, to do ls operation, pid=%d\n",pid);

      /* after run the following line, the child is ruturn */
 
	  
 /**********************************************/
 /* 	PLEASE LOOK AT THE FOLLOWING TWO CODE LINES!
	
	Qustion 2 -- Why the the second line -- printf ("The process ...) is not exacuted? 
    	
	Question 3 -- if you comment the execlp (..) What does the fork run?
	    	
*/

	execlp("/bin/ls","ls",NULL); 

     printf("The process is still in the child process, pid=%d\n",pid);

} else  
      {/* Question 4 -- The following line is printed early, why?*/
	  printf("This is the parent process, pid=%d\n\n",pid);
	  
	  wait(NULL); /* parent will wait for the child to complete*/
	  printf("\nChild Complete and back to Parent \n");
	}
	/* Question 5 -- to see the current pid, why it is now the parent pid? */
	printf("see what pid is now: pid=%d\n",pid);
	return 0;
}


// gcc P2-fork_additional.c -o P2-fork_additional.out && ./P2-fork_additional.out
