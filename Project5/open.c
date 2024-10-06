/*
 * COMP 3500: Project 5 Scheduling
 * Xiao Qin
 * Version 1.0
 *
 * COMP 3500: Project 5 Scheduling
 * Liam Maher
 * Note: No changes made, comment block added for submission.
 */
#include <stdio.h>
/*
 * Open file. To open a file using name as an input, file_descriptor is an output.
 */
int
open_file(char *name, FILE **file_descriptor)
{
    if (! (*file_descriptor = fopen(name, "r"))) {
        printf("File %s can't be opened. Please retry ...\n");
        return 1;
    }

    printf("Open file: %s\n", name);
    return 0;
}
