/*
 * COMP 3500: Project 5 Scheduling
 * Liam Maher
 * Version 1.0 06/19/2024
 *
 * This source code, as well as the comments included are
 * modeled after the source code and header files provided 
 * in the Canvas lecture materials by Xiao Qin.
 *
 *This is the header file for stats.c
 */


#ifndef _STATS_H_
#define _STATS_H_

/*
 * Function declarations for the caluclations in output.
 */
double avg_wait_time(task_t finished_tasks[], u_int count);
double avg_response_time(task_t finished_tasks[], u_int count);
double avg_turnaround_time(task_t finished_tasks[], u_int count);
double overall_cpu_usage(task_t finished_tasks[], u_int count);
void print_all_stats(task_t finished_tasks[], u_int count);

#endif
