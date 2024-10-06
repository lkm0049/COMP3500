/*
 * COMP 3500: Project 5 Scheduling
 * Xiao Qin
 * Version 1.0  11/18/2019
 *
 *
 * COMP 3500: Project 5 Scheduling
 * Liam Maher
 * Version 2.0 06/20/2024
 *
 * This source code shows how to conduct separate compilation.
 *
 * scheduler.h: The header file of scheduler.c
 */
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define MAX_TASK_NUM 32

typedef unsigned int u_int;

typedef struct task_info {
    u_int pid; //process identifier
    u_int arrival_time; //Arrival time
    u_int burst_time; //Burst time
    u_int start_time; //Start time for task
    u_int end_time; //End time for task
    u_int wait_time; //Wait time for Task
    u_int response_time; //Response time for task
    u_int remaining_time; //Time left for task
    u_int done; //Task is done
    u_int ready; //Task is ready
    u_int current_turn_time; //For RR procedure
} task_t;

//Function declarations
int no_tasks(task_t task_array[], u_int count);
int no_ready(task_t ready_queue[], u_int ready_count);
void setup(task_t task_array[], u_int count);
u_int update_ready_queue(task_t task_array[], u_int count, u_int clock, task_t ready_queue[], u_int ready_count); //IF ERROR OCCURS, MIGHT NEED TO MAKE READY_COUNT CALL BY REFERENCE
//Policy Function declarations
void fcfs(task_t task_array[], u_int count, task_t finished_tasks[]);
void srtf(task_t task_array[], u_int count, task_t finished_tasks[]);
void rr(task_t task_array[], u_int count, u_int time_quantum, task_t finished_tasks[]);

#endif
