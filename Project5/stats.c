/*
 *COMP 3500: Project 5 Scheduling
 *Liam Maher
 *Version 1.0 06/19/2024
 *
 * This file calculates the statistics to be used in the output
 * of this project.
 *
 * How to compile?
 * gcc -c stats.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

/*
 * avg_wait_time()
 *
 * params: finished_tasks[] - array of finished tasks.
 * 	   count - length of array (Ex. # of finished tasks)
 *
 *returns: avg_time -  average wait time for the tasks to be used in output
 *
 *Notes: Equation for wait time obtained through class lectures.
 *
 */
double avg_wait_time(task_t finished_tasks[], u_int count) {
	//initialize variables to be used in function
	double wait_time = 0; // wait time for current finished task
	double total_wait_time = 0; //Total wait time of all of the finished tasks
	double avg_time = 0; // average wait time of all finished tasks
	

	/*
 	 *Calculate the wait time
	 */
	int i;
	for (i=0; i < count; i++) {
		//wait_time = turnaround_time  (end_time - arrival_time) - busrt_time
		wait_time = finished_tasks[i].end_time - finished_tasks[i].arrival_time - finished_tasks[i].burst_time;
		//Add this value for each finished task into total_wait_time, in order to average
		total_wait_time += wait_time;
	}

	/*
 	 *Average the time.
	 *avg_wait_time = total_wait_time / count (number of finished tasks)
	 */
	avg_time = total_wait_time / count;

	//return average wait time
	return avg_time;

}

/*
 * avg_response_time()
 *
 * params: finished_tasks[] - array of finished tasks.
 * 	   count - length of array (Ex. # of finished tasks)
 *
 * returns: avg_resp_time - average response time to be used in the output
 *
 * Notes: Equation for response time obtained through class lectures.
 *
 */
double avg_response_time(task_t finished_tasks[], u_int count) {
	//initalize variables to be used in the function
	double response_time = 0; // response time for current finished task
	double total_response_time = 0; //total response time of all finished tasks
	double avg_resp_time = 0; //average response time of all finished tasks

	/*
 	 *Calculate the response time
	 */
	int i;
	for(i=0; i < count; i++) {
		//response_time = start_time - arrival_time
		response_time = finished_tasks[i].start_time - finished_tasks[i].arrival_time;
		//Add this value for each finished task into total_response_time, in order to average.
		total_response_time += response_time;
	}
	/*
 	 *Average the time.
	 *average_response_time = total_response_time / count (number of finished tasks)
	 */
	avg_resp_time = total_response_time / count;

	//return average wait time
	return avg_resp_time;
}

/*
 * avg_turnaround_time()
 *
 * params: finished_tasks[] - array of finished tasks.
 * 	   count - length of the array (Ex. # of finished tasks)
 *
 * returns: avg_turn_time -  average turnaround time to be used in the output.
 *
 * Notes: Equation for turnaround time obtained through class lectures.
 *
 */
double avg_turnaround_time(task_t finished_tasks[], u_int count) {
	//initialize variables to be used in the function
	double turnaround_time = 0; //turnaround time for current finished task
	double total_turnaround_time = 0; //total turnaround time of all finished tasks
	double avg_turn_time = 0; //average turnaround time of all finished tasks
	
	/*
 	 *Calculate turnaround time
	 */
	int i;
	for(i = 0; i < count; i++) {
		//turnaround_time = end_time - arrival_time
		turnaround_time = finished_tasks[i].end_time - finished_tasks[i].arrival_time;
		//Add this value for each finished task into total_turnaround_time, in order to average.
		total_turnaround_time += turnaround_time;
	}
	/*
 	 *Average the time.
	 *average_turnaround_time = total_turnaround_time / count (number of finished tasks)
	 */
	avg_turn_time = total_turnaround_time / count;

	//return average turnaround time
	return avg_turn_time;


}


/*
 *overall_cpu_usage()
 *
 * params: finished_tasks[] - array of finished tasks.
 * 	   count - length of the array (Ex. # of finished tasks)
 *
 * returns: cpu_percentage - percent usage of CPU
 *
 */
double overall_cpu_usage(task_t finished_tasks[], u_int count) {
	//initialize variables to be used in the function
	double cpu_time = 0;  // cpu time for current finished task
	double total_cpu_time = 0; // total cput time for all finished tasks
	double total_task_time = 0; //total time for tasks
	double cpu_percentage = 0; //percent utilization of cpu

	/*
 	 *Calculate cpu time
	 */
	int i;
	for (i=0; i < count; i++) {
		//cpu_time = burst time
		cpu_time = finished_tasks[i].burst_time; //cpu time for current finished task
		//Add this value for each finished task into total_cpu_time
		total_cpu_time += cpu_time;
	}
	/*
 	 *Calculate total task time
	 */
	int j;
	for (j=0; j < count; j++) {
		//Check if the end time for task is greater than total task time
		if (finished_tasks[j].end_time > total_task_time) {
			//if true, add this time total task time
			total_task_time = finished_tasks[j].end_time;
		}
	}
	/*
 	 *Calculate the cpu percentage
	 */
	cpu_percentage = (total_cpu_time / total_task_time) * 100;

	//return the cpu percentage
	return cpu_percentage;
}

/*
 * print_all_stats()
 *
 * params: finished_tasks[] - array of finished tasks.
 * 	   count - length of the array (Ex. # of finished tasks)
 *
 * returns: N/A (void function)
 *
 * Notes: Print statements are derived from the provided output example from canvas
 * from Xiao Qin.  I tried to implement the print statements with \t in order to align the outputs
 * correctly, however, no matter what combination I did it would not align properly, so I opted with 
 * guessing the amount of spaces Dr.Qin had in his print statements to align it correctly.
 *
 */
void print_all_stats(task_t finished_tasks[], u_int count) {
	//Initialize outputs of functions to variables for print statements
	double calculated_waiting_time = avg_wait_time(finished_tasks, count); //Calculate average waiting time
	double calculated_response_time = avg_response_time(finished_tasks, count); //Calculate average response time
	double calculated_turnaround_time = avg_turnaround_time(finished_tasks, count); //Calculate average turnaround time
	double calculated_cpu_percentage = overall_cpu_usage(finished_tasks, count); // Calculate the cpu usage percentage

	/*
 	 *Print Statements for statistics section of output
	 */
	
	//IF PRINT STATEMENTS DO NOT LINE UP LIKE EXAMPLE OUTPUT, CHECK TABS AFTER COLONS IN PRINT STATEMENT

	//Print string of equals signs to distinguish statistics section (copied from assignment details to ensure correct number)
	printf("============================================================\n");
	
	//Print Average waiting time
	printf("Average waiting time:    %.2f\n", calculated_waiting_time); //Only print to 2 decimal places as per example output

	//Print Average response time
	printf("Average response time:   %.2f\n", calculated_response_time); //Only print to 2 decimal places as per example output

	//Print Average turnaround time
	printf("Average turnaround time: %.2f\n", calculated_turnaround_time); //Only print to 2 decimal places as per example output

	//Print overall cpu usage
	printf("Overall CPU usage:       %.2f%\n", calculated_cpu_percentage); //Only print to 2 decimal places as per example output, also add % at end

	//Print string of equals signs to distinguish end of statistics section (copied from assignment details to ensure correct number)
	printf("============================================================\n");



}











































