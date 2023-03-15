
/*
   ----- 48450 -- week 2 lab handout 3 an advanced program for pork process  ------- 

fork() creates a new process by duplicating the calling process. The new process, referred to as the child, is an exact duplicate of the calling process.

On success, the PID of the child process is returned in the parent, and 0 is returned in the child. On failure, -1 is returned in the parent, in which case, no child process is created, and errno is set appropriately.

Modified procsmess.c from Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 
  --------------------------------------------------------------------------------------------
	Question 1 -- Draw a flow chat of this program including the parant process and child process. You can refer to the lecture notes in UTSOnline 
	Question 2 -- why the line 74, the printf (..) is shown early in the result screen?
	Question 3 -- why the line 41 is printed twice in the result screen?

*/ 

/* for printf() and perror() */
#include <stdio.h>
 /* for fork() */
#include <sys/types.h>
#include <unistd.h>
 /* for wait*() */
#include <sys/wait.h>

int main() {
    pid_t mypid, childpid;
    int status;
    
     /* what's our pid? */
    mypid = getpid();
    printf("Hi. I'm the parent process. My pid is %d.\n", mypid);
    
     /* create the child */
    childpid = fork();
    if ( childpid == -1 ) {
        perror("Cannot proceed. fork() error");
        return 1;
    }
    printf("\ncheck 'Childpids' in the first parent/child1 fork %d.\n", childpid);
    
    if (childpid  == 0) {
         /* then we're the child process "Child 1" */
        printf("Child 1: I inherited my parent's pid as %d.\n", mypid=getppid());
        
         /* get our pid: notice that this doesn't touch the value of parent's "mypid" value */
        mypid = getpid();
        printf("\nChild 1: getppid() tells my parent is %d. My own pid instead is %d.\n", getppid(), mypid);
               
         /* forks another child */
        childpid = fork();
        if ( childpid == -1 ) {
            perror("Cannot proceed. fork() error");
            return 1;
        }
        
        if (childpid == 0) {
             /* then we're the child process "Child 2"*/
            printf("\nChild 2: I hinerited my parent's PID as %d.\n", getppid());
            
            mypid = getpid();
            printf("\nChild 2: getppid() tells my parent is %d. My own pid instead is %d.\n", getppid(), mypid);
                              
            
        } else {
             /* this is still "Child 1", which waits for its child (Child 2) to exit */
            while ( waitpid(childpid, &status, WNOHANG) == 0 ) sleep(1);
            // man wait in linux terminal! 

            if ( WIFEXITED(status)) printf("\nChild1: Child 2 exited with exit status %d.\n", WEXITSTATUS(status));
            else printf("\nChild 1: child has not terminated correctly.\n");
        }
    } else {
         /* then we're the parent process, "Parent" */
        printf("\nParent: fork() went ok. My child's (Child 1) PID is %d\n", childpid);
         
         /* wait for the child to terminate and report about that */
        wait(&status);
        
        if ( WIFEXITED(status) ) printf("\nParent: child 1 (child 2 has completed) has exited with status %d.\n", WEXITSTATUS(status));
        else printf("\nParent: child has not terminated normally.\n");
    }
    
    return 0;
}

/*tip of WIFEXITED(status) defined in wait.h google for "how to find the include file of wait.h"- Obtain exit status of a child process

This macro queries the child termination status provided by the wait and waitpid functions. If the WIFEXITED macro indicates that the child process exited normally, the WEXITSTATUS macro returns the exit code specified by the child process.

*/
