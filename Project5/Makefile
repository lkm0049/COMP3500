#COMP 3500: Project 5 Scheduling
#Xiao Qin
#Version 1.0
#COMP 3500: Project 5 Scheduling
#Liam Maher
#Version 2.0
#Note: only added, stat.o, comment block added to comply with project description
#see README for a manual compilation process
CC = gcc
CFLAGS = -g
OBJ = open.o read.o print.o stats.o

scheduler: $(OBJ) scheduler.c
	@echo "Creating the scheduler ..."
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c %.h
	@echo "Creating object files ..."
	$(CC) -c $< -o $@

clean:
	@echo "Deleting object and binary files ..."
	rm -rvf *.o scheduler
