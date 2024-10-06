/*
 * COMP 3500: Project 5 Scheduling
 * Xiao Qin
 * Version 1.0  11/18/2019
 *
 * This source code shows how to conduct separate compilation.
 *
 * How to compile using Makefile?
 * $make
 *
 * How to manually compile?
 * $gcc -c open.c
 * $gcc -c read.c
 * $gcc -c print.c
 * $gcc -c stats.c
 * $gcc open.o read.o print.o stats.c  scheduler.c -o scheduler
 *
 * How to run?
 * Case 1: no argument. Sample usage is printed
 * $./scheduler
 * Usage: scheduler <file_name>
 *
 * Case 2: file doesn't exist.
 * $./scheduler file1
 * File "file1" doesn't exist. Please try again...
 *
 * Case 3: Input file
 * $./scheduler task.list
 * data in task.list is printed below...
 *
 *
 * COMP 3500: Project 5 Scheduling
 * Liam Maher
 * Version 2.0 06/21/2024
 * NOTE: The author would like to acknowledge the use of ChatGPT, a language model
 *	 developed by OpenAI, in the preparation of this assignment. The ChatGPT
 *	 was used in the following way(s) in this assignment: brainstorming approaches
 *	 to certain function implementation problems. Each part that used ChatGPT in the
 *	 brainstorming process is marked with a comment explaining why it was used, how it was used,
 *	 and what it helped me to understand further.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scheduler.h"
#include "print.h"
#include "open.h"
#include "read.h"
//Add include statement for stats.h
#include "stats.h"

/*
 * Function declarations
 */

int no_tasks(task_t task_array[], u_int count);
int no_ready(task_t ready_queue[], u_int ready_count);
void setup(task_t task_array[], u_int count);
u_int update_ready_queue(task_t task_array[], u_int count, u_int clock, task_t ready_queue[], u_int ready_count); //IF ERROR OCCURS, MIGHT NEED TO MAKE READY_COUNT CALL BY REFERENCE
//Policy Function declarations
void fcfs(task_t task_array[], u_int count, task_t finished_tasks[]);
void srtf(task_t task_array[], u_int count, task_t finished_tasks[]);
void rr(task_t task_array[], u_int count, u_int time_quantum, task_t finished_tasks[]);
void print_ready_queue(task_t ready_queue[], u_int ready_count);


/*
 * Main function for program
 */


int main( int argc, char *argv[] )  {
    char *file_name; /* file name from the commandline */
    FILE *fp; /* file descriptor */
    task_t task_array[MAX_TASK_NUM];
    task_t stat_array[MAX_TASK_NUM];

    int error_code;
    u_int i;
    u_int count;
    u_int time_quantum = 0;

    //Need 3 or 4 arguments passed for functions
    if (argc < 3 || argc > 4) {
        printf("ERROR: Must have 3 or 4 Arguments\n");
        return EXIT_FAILURE;
    }
    //For RR policy, make sure you get necessary params
    if(strcmp(argv[2], "RR") == 0) {
	if(argc == 4) {
		printf("Time quantum is: %s\n", argv[3]); //Print what the time quantum value is
		time_quantum = atoi(argv[3]);
	}
	//Not 4 arguements
	else {
		printf("You need to enter a time quantum for RR policy\n");
		return EXIT_FAILURE;
	}
    }
    //Add to get and print file_name
    file_name = argv[1];
    error_code = open_file(argv[1], &fp);
    if (error_code == 1)
        return EXIT_FAILURE;

    printf("Scheduling Policy: %s\n", argv[2]);

    read_file(fp, task_array, &count);
    print_task_list(count, file_name);

    //Print bar to seperate sections
    printf("==================================================================\n");

    /*
     *Determine which Policy to Run
     */
     if(strcmp(argv[2], "FCFS") == 0) {
	//Run FCFS Policy
	fcfs(task_array, count, stat_array);
     }
     else if (strcmp(argv[2], "SRTF") == 0) {
	srtf(task_array, count, stat_array);
     }
     else if (strcmp(argv[2], "RR") == 0) {
	if (time_quantum <= 0) {
		printf("ERROR: Time quantum needs to be > 0.\n");
		return EXIT_FAILURE;
	}
	rr(task_array, count, time_quantum, stat_array);
     }
     else {
	printf("Invalid Policy Name.\n");
	return 0;
     }

    //Policy complete, Print Statistics
    print_all_stats(stat_array, count);

    fclose(fp);
    return EXIT_SUCCESS;
}

/*
 * Helper functions for the policies.
 * Check if tasks have not entered ready queue / tasks are still to be completed.
 */


/*
 *no_tasks()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of the array (Ex. Number of tasks)
 *
 * returns: 0 - task array is not empty
 * 	    1 - task array is empty
 *
 * Note: Helper function to note if all of the tasks are already in the ready queue or not.
 */
int no_tasks(task_t task_array[], u_int count) {
	//Variable declarations for function
	int tasks_ready = 0; //the number of tasks that are ready

	/*
 	 *Check if tasks have all entered ready queue
	 */
	int i;
	for (i=0; i < count; i++) {
		//Check if each of the tasks is ready
		if(task_array[i].ready == 1) {
			tasks_ready++; //task i is ready. increment number of ready tasks
		}
	}

	/*
 	 *Check if all tasks have entered the ready queue.
	 * (true if tasks_ready == count)
	 */
	if (tasks_ready == count) {
		return 1; //All tasks are in the ready queue
	}
	else { //Not all tasks are in the ready queue
		return 0; //Return 0 to signal that tasks are still waiting to be added to queue
	}
}

/*
 * no_ready()
 *
 * params: ready_queue[] - queue of tasks that are in ready state
 * 	   ready_count - Number of tasks in the ready queue
 *
 *returns: 0 - Ready Queue contains tasks (not empty)
 *	   1 - Ready Queue does not have any tasks (empty)
 *
 * Note: Helper function to note if there are tasks within the ready queue.
 *
 */
int no_ready(task_t ready_queue[], u_int ready_count) {
	//Variable declarations for function
	int tasks_done = 0; //number of tasks that are done

	/*
 	 *Calculate number of tasks done
	 */
	 int i;
	 for(i=0; i < ready_count; i++) {
		//Check if each task in queue is done
		if(ready_queue[i].done == 1) {
			tasks_done++; //task i in ready queue is done.  increment number of tasks done
		}
	}

	/*
 	 *Check if all of the tasks in ready queue are done.
	 *(true if tasks_done == ready_count)
	 */
	if (tasks_done == ready_count) {
		return 1; //All tasks in ready queue are done
	}
	else { //Not all tasks in ready queue are done
		return 0; //Tasks within ready queue that are not completed
	}

}

/*
 * setup()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of array (Ex. # of tasks)
 *
 * returns: N/A (void function)
 *
 * Notes: Used to avoid repetative code within the policies.
 * 	  Initializes the states of the tasks.
 *
 */
void setup(task_t task_array[], u_int count) {
	//Set tasks to needed initialization state.
	int i;
	for (i=0; i < count; i++) {
		task_array[i].ready = 0; //Task i is not ready
		task_array[i].done = 0; //Task i is not done
		task_array[i].current_turn_time = 0; //Only for rr method	
	}

} 

/*
 * update_ready_queue()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of array (Ex. # of tasks)
 * 	   clock - used within policies
 * 	   ready_queue[] - queue of ready tasks
 * 	   ready_count - length of ready queue (Ex. # of ready tasks)
 *
 * returns: ready_count - the new length of ready queue
 *
 * Notes: Used to avoid redundant code in policies. checks if task is
 * 	  ready to be placed in ready queue. While working on this part,
 * 	  I was unable to figure out if I made ready_count call by reference,
 * 	  how that would affect the methods I had already written with ready_count.
 * 	  Due to the strict time constraints on this project, I asked ChatGPT if I could avoid
 * 	  using pointers by making a the function return ready_count after using it.  I utilized ChatGPT
 * 	  to see how I could change my function from a void function to returning ready_count, which is an u_int.
 * 	  The only lines added from ChatGPT where function type (void -> u_int) and return ready_count;
 * 	  This seemed like an easy implementation at first, however, I got confused on if the policies would be able to see the
 * 	  updated ready_queue, and ChatGPT was able to help me understand that arrays are passed by reference by default in 
 * 	  C, which I was unsure of before, and helped me soldifiy my confidence in my implementation.
 *
 */
u_int update_ready_queue(task_t task_array[], u_int count, u_int clock, task_t ready_queue[], u_int ready_count) {
	//See if each task within the array needs to be placed in the ready queue
	int i;
	for (i = 0; i < count; i++) {
		//Has it arrived && is it not ready yet
		if ((task_array[i].arrival_time <= clock) && (task_array[i].ready == 0)) {
			task_array[i].ready = 1; //Make task i ready
			task_array[i].remaining_time = task_array[i].burst_time;
			ready_queue[ready_count] = task_array[i]; //Add task i to the ready queue
			ready_count++; //Task i is added to queue, so we must increment the ready_count
		}

	}
	return ready_count; //Added with help of ChatGPT
}
/* POLICIES */

/*
 * fcfs()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of array (Ex. # of tasks)
 * 	   finished_tasks[] - array of finished tasks.
 *
 * returns: N/A (void function)
 *
 * Notes: First Come First Serve (FCFS) scheduling policy.  Approach derived from
 * lecture materials on the alogorithm of FCFS.
 *
 */
void fcfs(task_t task_array[], u_int count, task_t finished_tasks[]) {
	//Variable declarations for function
	u_int clock = 0; //Clock for functions
	u_int ready_count = 0; //Number of functions ready
	u_int done_count = 0; //Number of functions done
	task_t task; //Tasks
	task_t ready_queue[count]; //Ready queue init

	/*
 	 *Initialize the task to not ready / done
	 */
	setup(task_array, count);
	

	/*
 	 *Continue FCFS policy until the task array and ready queue are empty.
	 */
	while(!no_tasks(task_array, count) || !no_ready(ready_queue, ready_count)) {
		//Determine if each task needs to be in ready queue, get the updated value of ready_count after
		ready_count = update_ready_queue(task_array, count, clock, ready_queue, ready_count);
		//Check if the ready queue is empty 
		if (!no_ready(ready_queue, ready_count)) {
			//Find first task in ready queue that is not done
			int i;
			int index = -1; //Initialized to -1 to make sure no index is chosen if not ready
			for (i = 0; i < ready_count; i++) {
				if(ready_queue[i].done == 0) {
					task = ready_queue[i];
					index = i;
					break;
				}
	
			}
	
			//If task has not had cpu time, then set its start time
			if(task.remaining_time == task.burst_time) {
				task.start_time = clock; //set start time
			}	

			task.remaining_time--; // Task started, now dec remaining time
			printf("<time %u> process %u is running\n", clock, task.pid);  //Print task is running at specific time
			clock++; //increment the clock
			ready_queue[index] = task; //Update ready queue

			//Determine if task is finished
			if(task.remaining_time == 0) {
				printf("<time %u> process %u is finished...\n", clock, task.pid); //Print task is done at specific time
				task.end_time = clock; //Set the end time to clock
				ready_queue[index].done = 1; //Set task to done
				finished_tasks[done_count] = task; //Add task to finished tasks
				done_count++; // increment done_count

			}
		}
		else {
			printf("<time %u> no process is running\n", clock); //ready queue is empty, print none are running
			clock++; //increment the clock
		}	
	}
	//Process is finished, print that tasks are finished
	printf("<time %u> All processes finish ......\n", clock);

	
	
}

/*
 * srtf()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of array (Ex. # of tasks)
 * 	   finished_tasks[] - array of finished tasks
 *
 * returns: N/A (void function)
 *
 * NOTES: Implementation of the Shortest Remaining Time First Scheduling policy.
 * Algorthinms and approach derived from in class discussions and handouts regarding the
 * implementation of SRTF.
 *
 */

void srtf(task_t task_array[], u_int count, task_t finished_tasks[]) {
	//Variable declarations for function
	u_int clock = 0; //Clock for policy
	u_int ready_count = 0; //Number of ready tasks
	u_int done_count = 0; //Number of done tasks
	task_t task; //tasks
	task_t ready_queue[count]; //initialize ready queue to have correct number indeces

	/*
 	 * Initialize tasks to not read/done
 	 */
	setup(task_array, count);

	/*
 	 *Continue SRTF policy until ready queue and tasks are empty
	 */
	
	 while(!no_tasks(task_array, count) || !no_ready(ready_queue, ready_count)) {
		//Determine if each task is ready to be placed in ready queue
		ready_count = update_ready_queue(task_array, count, clock, ready_queue, ready_count); //Obtain ready_count value from helper function

		//Determine if ready queue is empty
		 if (!no_ready(ready_queue, ready_count)) {
 			//Initialize shortest time remaininng as large value
 			u_int shortest_time_remaining = 10000; //By setting to a large number instead of 0, we can make sure we actually select a shorter time
			/*
 			 *Check for shortest time remaining in ready queue
			 */
			int i;
			int index;
			for(i = 0; i < ready_count; i++) {
				//Check if task is done
				if(ready_queue[i].done == 0) {
					//Check if task is shorter than shortest time remaining
					if(ready_queue[i].remaining_time < shortest_time_remaining) {
							task = ready_queue[i]; //Set the task to ith task
							shortest_time_remaining = task.remaining_time; //update remaining time
							index = i;
					}
				}
			}

			//If task has not had CPU time yet, then set the start time
			//Task should be the task with shortest remaining time
			if(task.remaining_time == task.burst_time) {
				//Set start time
				task.start_time = clock;
			}

			task.remaining_time--; //Task started, dec remaining time
			printf("<time %u> process %u is running\n", clock, task.pid); //Print that task is running
			clock++; //increment the clock
			ready_queue[index] = task; //update ready queue
 			
			/*
 			 *Check if Task is done
			 */
			 if(task.remaining_time ==0) {
				printf("<time %u> process %u is finished...\n", clock, task.pid); //Print process is finished since no remaining time
				task.end_time = clock; //Set the end time for task
				ready_queue[index] = task; //update ready queue
				ready_queue[index].done = 1; //set to done
				finished_tasks[done_count] = ready_queue[index]; //Add task to finished array
				done_count++;
			 }
			                	                                                                                                                                                                                                                                                                                                                     
		 }
		 else {
			//Ready queue is empty, no running tasks
			printf("<time %u> no process is running\n", clock);
			clock++; //increment the clock

		 }
	}
	//All Tasks done
	printf("<time %u> All processes finish ......\n", clock);



}

/*
 * rr()
 *
 * params: task_array[] - array of tasks
 * 	   count - length of array (Ex. # of tasks)
 * 	   time_quantum - length of time
 * 	   finished_tasks[] - array of finished tasks
 *
 * returns: N/A (void function)
 *
 * NOTES: Implementation of the Round Robin Scheduling policy.
 * Algorithms and techniques derived from in class discussions, 
 * handouts, and powerpoints reagarding the RR policy.
 *
 */

void rr(task_t task_array[], u_int count, u_int time_quantum, task_t finished_tasks[]) {
	//Variable declarations for function
	u_int clock = 0; //Clock for function
	u_int ready_count = 0; //Number of Ready tasks
	u_int done_count = 0; //Number of done tasks
	task_t ready_queue[count]; //init ready queue
	int current_task_index = 0; //Keep the current task index in order to implement rr
	int i; // used for for loops
	/*
 	 *Initialize tasks to not ready/done
	 */
	setup(task_array, count);
	
	/*
 	 * Continue RR policy until ready queue and tasks are empty
 	 */ 
	while(!no_tasks(task_array, count) || !no_ready(ready_queue, ready_count)) {

		//Determine if each task should be placed in ready queue
		ready_count = update_ready_queue(task_array, count, clock, ready_queue, ready_count);

		//Debug statement
		//print_ready_queue(ready_queue, ready_count); //PLACEMENT HELP FROM CHATGPT
		/*
 		 *Determine if ready Queue is empty, if not find first not-done task.
		 */
		if (!no_ready(ready_queue, ready_count)) {
			//Ready queue is not empty, find first not-done task
			task_t task = ready_queue[current_task_index];

			/*
 			 *If task has not had cpu time, set its start time
			 */
			if (task.remaining_time == task.burst_time) {
				//Task hasn't started, set start time
				task.start_time = clock;
			}

			/*
 			 *Task started, start RR procedures
			 */
			 u_int time_holder = 0; //Used to ensure task only holds cpu for <= time_quantum
			 //While the task has held cpu for less than time quantum and still has time remaining
			 while (time_holder < time_quantum && task.remaining_time > 0) {
				 task.remaining_time--; //Task started, dec remaining time
				 task.current_turn_time++; //Increment cpu time for process
				 printf("<time %u> process %u is running\n", clock, task.pid);
				 clock++; //Increment clock
				 time_holder++; //increment time hed

				 ready_count = update_ready_queue(task_array, count, clock, ready_queue, ready_count);
			}

			//Update task in ready queue
			ready_queue[current_task_index] = task;

			/*
 			 *Check if process is finished.
			 */
			if(task.remaining_time == 0) {
				//Task finished, print
				printf("<time %u> process %u is finished...\n", clock, task.pid);
				task.end_time = clock; // set end time
				task.done = 1; //set task to done
				finished_tasks[done_count] = task;  //Add task to finished task list
				done_count++;

				//Remove the task from ready queue
				for (i = current_task_index; i < ready_count -1; i++) {
					ready_queue[i] = ready_queue[i+1]; // shift ready queue
				}
				ready_count--; //Decrement number of ready tasks
				if (current_task_index >= ready_count) {
					current_task_index = 0; //reset the index to go through rest of processes correctly
				}
			}
			else {
				//task has remaining time
				ready_queue[current_task_index] = task;
				task_t temp = ready_queue[current_task_index];
				for (i = current_task_index; i < ready_count - 1; i++) {
					ready_queue[i] = ready_queue[i+1]; //shift ready queue
				}
				ready_queue[ready_count - 1] = temp;
				/*
 				 *The following line was added after the debug print statements.
 				 */
				current_task_index = 0; //go to first in ready queue		
			}		
			//Debug statement
			//print_ready_queue(ready_queue, ready_count);  //PLACEMENT HELP FROM CHATGPT
		}
		//Ready Queue is empty
		else {
			printf("<time %u> no process is running\n", clock);
			clock++; //increment the clock
		}
		
	}	
	/*
 	 *All processes finished
	 */
	printf("<time %u> All processes finish ......\n", clock);	


}

/*
 * print_ready_queue()
 *
 * params - ready_queue[] - ready queue
 *	  - ready_count - # of ready tasks
 *
 * returns: N/A (void fn)
 *
 * NOTE: This is not necessary for the implementation of this project, I added this 
 * 	 function in order to debug my rr implementation, as it appears I have a problem
 * 	 manipulating the order in which tasks should be evaluated. I spent maybe 5 hours straight 
 * 	 trying to fix my rr implementation and could not for the life of me figure out why it was not
 * 	 working.  I eventually knew I needed to add debug statements as the problem was with the ordering
 *	 or access of my queues.  I asked ChatGPT how I could go about adding the print statements.  I used ChatGPT
 *	 to locate where I should place these debug statements because I was manipulating the ready queue a lot and wanted
 *	 to put them in meaningful places.  Although the print function is not from chatGPT, it instructed me to place the statements
 *	 after I had exited my loops (to not continously print meaningless information).  I then was able to realize I was not
 *	 starting from the first task in the index.  The added line of code after this is marked with a comment.  This helped me to learn that
 *	 I should definetly get used to adding more debug statements in my code because I just wasted hours of my life on something very 
 *	 simple.
 *
 */
void print_ready_queue(task_t ready_queue[], u_int ready_count) {
	int i;
	printf("Ready Queue: ");
	for (i = 0; i < ready_count; i++) {
		printf("%u ", ready_queue[i].pid);
	}
	printf("\n");

}















































