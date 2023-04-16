/*
This is a program to do the shortest-remaining time first (SRTF) CPU scheduling

               Process			Arrival Time		Burst Time
                P1			        8                   10
                P2              10                  3
                P3              14                  7
                P4              9                   5
                P5              16                  4
                P6              21                  6
                P7              26                  2

sort: 2   3   4   5   6   7   10
      P7, P2, P5, P4, P6, P3, P1
Through this Lab sample. Please answer the following question that is embeded in the code lines.

Question-1: what does the sort function achieve? Give your analysis by commenting line by line in the function call.

This is a simple version 1) sort the processes based on SRTF, 2) calcuate the start time (ST) for each process 3) based on ST, the process arrival time (AT) and the process bust time (BT) to calculate the process wait time and turn round time. The program has some limitation to handle more complicated SRTF cases for the arrival time setting.
gcc A3.c -o a3.o -lpthread -lrt -Wall && ./a3.o
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/* a struct storing the information of each process */
typedef struct
{
  int pid; // process id
  float arrive_t, wait_t, burst_t, turnaround_t;
  float start_t; /* process time */
} process;

typedef struct
{
  float avg_wait_t, avg_turnaround_t;
} avg_wait_t_and_avg_turnaround_t;

void sort(process p[], int start, int num);
void initProcess(process *pro, int pid, float arrive_t, float burst_t);

avg_wait_t_and_avg_turnaround_t *SRTF(process p[], int num);
/******************* Main function ******************/

int main()
{
  int processNum = 7; // process number

  /* store process information using our struct(include id, four types of time) */
  process *pro;

  /*------initialize the data: process id and time -------*/
  printf("This is a program to run example data to show shortest-remaining  job first (SRJF) CPU scheduling.\n\n");
  pro = malloc(sizeof(process) * processNum); /* allocate memory for variable process */
  // Process 1
  initProcess(&pro[0], 1, 8, 10);
  // Process 2
  initProcess(&pro[1], 2, 10, 3);
  // Process 3
  initProcess(&pro[2], 3, 14, 7);
  // Process 4
  initProcess(&pro[3], 4, 9, 5);
  // Process 5
  initProcess(&pro[4], 5, 16, 4);
  // Process 6
  initProcess(&pro[5], 6, 21, 6);
  // Process 7
  initProcess(&pro[6], 7, 26, 2);

  avg_wait_t_and_avg_turnaround_t *res;
  res = SRTF(pro, 7);
  printf("\nres Average wait time: %f", res->avg_wait_t);
  printf("\nres Average turnaround time: %f\n", res->avg_turnaround_t);
  printf("hello0");

  int fd;
  printf("hello1");
  // FIFO file path
  char *myfifo = "/tmp/myfifo";
  printf("hello2");

  // Creating the named file(FIFO)
  mkfifo(myfifo, 0666);
  printf("hello3");

  char arr1[11], arr2[11];

  // Open FIFO for write only
  fd = open(myfifo, O_WRONLY);
  return 0;

  // Write avg_wait_t and avg_turnaround_t to FIFO
  // and close it
  char avg_wait_t_buf[11];
  char avg_turnaround_t_buf[11];
  gcvt(res->avg_wait_t, 10, avg_wait_t_buf);
  gcvt(res->avg_turnaround_t, 10, avg_turnaround_t_buf);
  write(fd, avg_wait_t_buf, 11);
  write(fd, avg_turnaround_t_buf, 11);
  printf("write avg_wait_t_buf : %s\n", avg_wait_t_buf);
  printf("write avg_turnaround_t_buf : %s\n", avg_turnaround_t_buf);
  close(fd);

  // Open FIFO for Read only
  fd = open(myfifo, O_RDONLY);

  // Read from FIFO
  read(fd, arr1, 11);
  read(fd, arr2, 11);

  // Print the read message
  printf("Read avg_wait_t : %s\n", arr1);
  printf("Read avg_turnaround_t : %s\n", arr2);
  close(fd);

} /* end of main */

/******************* Call function ******************/

void sort(process p[], int start, int num)
{
  int i, j;
  process temp;
  for (i = start; i < num; i++)
  {
    for (j = i + 1; j < num; j++)
    {

      if (p[i].burst_t > p[j].burst_t)
      {
        temp = p[i];
        p[i] = p[j];
        p[j] = temp;
      }
    }
  }
} /* the sort results in the end = P7, P2, P5, P4, P6, P3, P1 */

avg_wait_t_and_avg_turnaround_t *SRTF(process pro[], int processNum)
{
  int i;
  /* to declare variables
  1)average waiting time,
  2) turnaround time and
  3)global start time and set up initial value = 0.0
  */
  float avg_wait_t = 0.0, avg_turnaround_t = 0.0;
  int Current_process_start = 0;

  float time_residue = pro[0].arrive_t + 1; /* time_residue = 1 due to the P1 arrives at time = 0 */

  /*----------- sort the existing processes ------------*/
  sort(pro, 0, processNum); /* P7, P2, P5, P4, P6, P3, P1 */

  /*--- implementing the SRJF algorithm on burst time ---*/
  for (i = 0; i < processNum; i++)
  {
    if (pro[i].arrive_t <= Current_process_start)
    { // set the rest process' start time
      if (pro[i].arrive_t == 0)
        Current_process_start -= time_residue; /* only for the P1 process */
      pro[i].start_t = Current_process_start;  // set up the current process' start time
      printf("Process ID=%d, start_time=%f\n", pro[i].pid, pro[i].start_t);
    }
    else
    {
      pro[i].start_t = pro[i].arrive_t; // set the shortest burst process' start time

      if (pro[i].arrive_t > 0)
        Current_process_start += time_residue;
      printf("Process ID=%d, start_time=%f\n", pro[i].pid, pro[i].start_t);
    }

    /*-- set the global start time to the end of the process done time --*/
    Current_process_start += pro[i].burst_t;
    /*set the wait time as CPU start time minus process arrive time*/

    pro[i].wait_t = pro[i].start_t - pro[i].arrive_t;
    printf("Process ID=%d, process start_time=%f, process arrive time=%f \n", pro[i].pid, pro[i].start_t, pro[i].arrive_t);

    /* set turn around time as bust time plus wait time */
    pro[i].turnaround_t = pro[i].burst_t + pro[i].wait_t;
    avg_wait_t += pro[i].wait_t;
    avg_turnaround_t += pro[i].turnaround_t;
    printf("Process ID=%d, p-bust-t=%f,p-wait-t=%f, p-trunaround-t=%f \n\n", pro[i].pid, pro[i].burst_t, pro[i].wait_t, pro[i].turnaround_t);
  }

  /* --------------Display the results------------------ */
  avg_wait_t /= processNum;
  avg_turnaround_t /= processNum;
  printf("Process Schedule Table: \n");
  printf("\tProcess ID\tArrival Time\tBurst Time\tWait Time\tTurnaround Time\n");
  for (i = 0; i < processNum; i++)
    printf("\t%d\t\t%f\t%f\t%f\t%f\n", pro[i].pid, pro[i].arrive_t, pro[i].burst_t, pro[i].wait_t, pro[i].turnaround_t);
  printf("\nAverage wait time: %f", avg_wait_t);
  printf("\nAverage turnaround time: %f\n", avg_turnaround_t);
  avg_wait_t_and_avg_turnaround_t *res;
  res->avg_wait_t = avg_wait_t;
  res->avg_turnaround_t = avg_turnaround_t;
  return res;
}

void initProcess(process *pro, int pid, float arrive_t, float burst_t)
{
  pro->pid = pid;
  pro->arrive_t = arrive_t;
  pro->burst_t = arrive_t;
}
