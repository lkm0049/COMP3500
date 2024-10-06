/*
 * COMP 3500: Project 5 Scheduling
 * Xiao Qin
 * 
 * This source code shows how to pass commandline arguments to your progrm
 *
 * How to compile? 
 * $gcc command.c -o command
 *
 * How to run?
 * Case 1: no argument. Sample usage is printed
 * $./command
 * Usage: command file_name [FCFS|RR|SRTF] [time_quantum]
 *
 * Case 2: 2 arguments. 
 * $./command file1 FCFS 
 * The file name is: file1
 * The chosen policy is: FCFS
 *
 * Case 3: 2 arguments.
 * $./command file2 RR
 * The file name is file2
 * The chosen policy is RR
 * Please enter time_quantum for the RR policy!
 */
#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] )  {

    if (argc == 1 || argc > 4) {
        printf("Usage: command file_name [FCFS|RR|SRFT] [time_quantum]\n");
        return 0;
    }
    
    /* Print file name */
    printf("The file name is %s\n", argv[1]);

    /* Print policy */
    printf("The chosen policy is %s\n", argv[2]);

    /* Handle the RR policy */
    if (strcmp(argv[2], "RR") == 0) {
        if (argc == 4)
            printf("time_quantum is set to %s\n", argv[3]);  
        else printf("Please enter time_quantum for the RR policy!\n");
    }
}
