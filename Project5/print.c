/*
 *COMP 3500: Project 5: Scheduling
 *Version 1.0
 *Xiao Qin
 *
 *COMP 3500: Project 5: Scheduling
 *Version 2.0 06/20/2024
 *Liam Maher
 *
 * Note: The instructions state that our output "must follow the specified output format".
 * In order to do this, this file, which was provided by Dr. Qin, must be changed.  The only changes
 * made are to match the specified output format provided in the Project 5 handout.
 *
 */
#include <stdio.h>
#include "scheduler.h"

/*
 * Print a task list. To display the task list, the length of which is specified by size.
 */
void
print_task_list(int size, const char* file_name)
{
    int i;
    printf("There are %d tasks loaded from \"%s\". Press any key to continue ...", size, file_name);
    /* print task_array to check input file */
    //This is in Dr.Qins provided files, but not in the specified format, so I commented it out
    //for (i = 0; i < size; i++)
      //  printf("Task PID: %u, Arrival Time: %u, Burst Time: %u\n", task_list[i].pid, \
       //    task_list[i].arrival_time, task_list[i].burst_time);

   // printf("Press any key to continue ...\n");
    getchar();
}
