/*
 * COMP 3500: Project 5 Scheduling
 * Xiao Qin
 * Version 1.2  11/14/2019
 *
 * This source code shows how to pass commandline arguments to your progrm
 *
 * How to compile? 
 * $gcc input.c -o command
 *
 * How to run?
 * Case 1: no argument. Sample usage is printed
 * $./input
 * Usage: input <file_name> 
 *
 * Case 2: file doesn't exist. 
 * $./input file1 
 * File "file1" doesn't exist. Please try again... 
 *
 * Case 3: Input file
 * $./input task.list
 * data in task.list is printed below...
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASK_NUM 32

typedef unsigned int u_int;

typedef struct task {
    u_int pid;
    u_int arrival_time;
    u_int burst_time;
} task_t;

int main( int argc, char *argv[] )  {
    char *file_name; /* file name from the commandline */
    FILE *fp; /* file descriptor */
    task_t task_array[MAX_TASK_NUM]; 
    
    u_int i;
    u_int count;

    if (argc != 2) {
        printf("Usage: input <file_name>\n");
        return EXIT_FAILURE;
    }
    
    file_name = argv[1];
    if (! (fp = fopen(file_name, "r"))) {
        printf("File %s can't be opened. Please retry ...\n");
        return EXIT_FAILURE;
    }

    printf("Open file: %s\n", file_name);

    /* read data from input file */
    count = 0;
    while (fscanf(fp, "%u %u %u", &task_array[count].pid, &task_array[count].arrival_time, \
                  &task_array[count].burst_time)!= EOF) {
        count++;
    }
 
    printf("There are %u tasks loaded from %s ...\n", count, file_name);

    /* print task_array to check input file */
    for (i = 0; i < count; i++)
        printf("Task PID: %u, Arrival Time: %u, Burst Time: %u\n", task_array[i].pid, \
               task_array[i].arrival_time, task_array[i].burst_time);

    printf("Press any key to continue ...\n");
    getchar();

    fclose(fp);
    return EXIT_SUCCESS;
}
