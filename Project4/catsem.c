/*
 * catsem.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use SEMAPHORES to solve the cat syncronization problem in 
 * this file.
 */
/*
 *NOTE FOR SUBMISSION: While working on this project at my work, the wifi crashed while 
 * I was editting the file.  When I was able to reaccess the file, I was prompted with a 
 * recovery message and a message to delete "catsem.c.swp" (might not have been the direct
 * name of the file but it was a .swp file I know that). I followed the instructions presented, 
 * and also checked my asst1 directory to make sure the only file is catsem.c, as of now, that is
 * all i see.  I wanted to make a note of this incase there were any error messages or if it affected the 
 * diff file that we must submit.  This issue occured yet again when the Auburn server crashed while editting my file.
 * I spoke with Dr. Qin on Piazza about it and he said that if there are any problems with the diff file, which he doesn't think
 * there will be, that he would have you look at the catsem.c file itself.  Even if I don't notice any problems with the diff file,
 * I am going to include my catsem.c in the tar file as well just in case.  Thank you so much, and sorry for any inconvience.
 */

/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
/*Add the following include statement for semaphore usage*/
#include <synch.h>

/*
 * 
 * Constants
 *
 */

/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2

/*
 *Number of animals.
 */
#define NANIMALS 8

/*
 *Max number of times animal may eat
 */
#define NEAT 5

/*
 * Max time for animal to play & eat
 */
#define MTIME 3

/*
 *Boolean values to be used in the program
 */
#define true 1
#define false 0
typedef char bool;

/*
 * Variable declarations from power point
 */
static struct semaphore *done;
static struct semaphore *mutex;
static struct semaphore *dish_mutex;
static struct semaphore *cats_queue;
static struct semaphore *mice_queue;
static struct semaphore *cats_done;
static struct semaphore *mice_done;
/*
 * Volatile variable declarations
 */
volatile int cats_wait_count = 0; //init as 0
volatile bool no_cat_eat = true; //init as true
volatile int mice_wait_count = 0; //init as 0
volatile bool no_mouse_eat = true; //init as true
volatile bool all_dishes_available = true; //init as true
volatile bool dish1_busy = false; //init as false
volatile bool dish2_busy = false; //init as false

/*
 * Cat/Mouse name declarations
 */
static const char *catnames[6] = {"charles", "cam", "conner", "christain", "caleb", "carson"}; //All Cat names start with "c" for simplicity when viewing output
static const char *mousenames[2] = {"mickey", "minney"};  //All Mouse names start with "m" for simplicity when viewing output


/*
 * 
 * Function Definitions
 * 
 */

static void setup();
static void cleanup();

/*
 * catsem()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      All print statements were made to model the catsem.script handout,
 *      I was unsure if this was necessary for grading purposes, but
 *      it made it much easier for me to see if my output was performing as needed.
 *      Function code was taken from various handouts, as well as the semaphore
 *      implementation powerpoint provided.
 */

static
void
catsem(void * unusedpointer, 
       unsigned long catnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) unusedpointer;
	(void) catnumber;  //might be able to remove this here, CHECK
	/*
 	 *Print cat is starting
 	 */
	kprintf("... %s starting.\n", catnames[catnumber]);


	/*
 	 *Number of times cat will eat
	 *Attempted to initialize i within the for loop, but got "declaration outside of 
	 * C99 mode error", I was able to find a fix for this online using gcc but did not
	 * want that to potentially lead to complications with future projects.
         */
	int i;
	for (i = 0; i < NEAT; i++) {
		bool first_cat_eat = false;
		bool another_cat_eat = false;
		int mydish = 0; //Dish for the cat


		/*
 		 *Set the max time for a cat to play
 		 */
		//clocksleep(random() % MTIME);

		/*
 		 *Print a certain cat is hungry
		 */
		kprintf("    %s feels hungry.\n", catnames[catnumber]);
		/*
 		 *Allow the first waiting cat in.
		 *Use of P and V from synch.h import.
 		 */
		P(mutex);
		if (all_dishes_available == true) {
			all_dishes_available = false; //Door open, no one is eating
			V(cats_queue); //Allow first cat in
		}
		cats_wait_count++;
		V(mutex);

		/*
 		 *First cat is in the kitchen, next cat is still waiting.
 		 */
		P(cats_queue);
	
		/*
 		 *If no cat eats still true, first cat
 		 */
		if (no_cat_eat == true) {
			no_cat_eat = false;
			//Set first cat eat to true
			first_cat_eat = true;
		} else { 
			first_cat_eat = false; //not first cat eating
		}
		/*
	         *First cat allows another cat in
		 */
		if (first_cat_eat == true) {
			P(mutex);
			if (cats_wait_count > 1) {
				another_cat_eat = true; /*Let other cat eat*/
				V(cats_queue);
			}
			V(mutex);
		}
		kprintf(">>>  %s enters the kitchen.\n", catnames[catnumber]);
		/*
 		 *Assign dish to cat
		 */
		P(dish_mutex);
		if (dish1_busy == false) {
			dish1_busy = true;
			//Assign dish 1 to cat
			mydish = 1;
		} else {
			assert(dish2_busy == false);
			dish2_busy = true;
			//Assign dish 2 to cat
			mydish = 2;
		}
		V(dish_mutex);

		/*
	 	 *Allow cat to eat the dish
		 */
		 kprintf("***  %s starts  eating at dish %d.\n", catnames[catnumber], mydish);
		 clocksleep(random() % MTIME); /*enjoy food*/
		 kprintf("*** %s done eating at dish %d.\n", catnames[catnumber], mydish); /*done*/

		/*
  		 *Release the dishes
		 */
		P(mutex); 
		P(dish_mutex);
		if (mydish == 1)  { //Check if cat dish is 1 and release
			dish1_busy = false;
			kprintf("*** %s is releasing dish %d.\n", catnames[catnumber], mydish);
		} else {
			assert(mydish == 2); //make sure cat is holding dish 2
			dish2_busy = false; //release dish 2
			kprintf("*** %s is releasing dish %d.\n", catnames[catnumber], mydish);
		}
		V(dish_mutex);
		cats_wait_count--; //decrement number of waiting cats
		V(mutex);

		/*
		 *First cat leaces kitchen, will also switch the turn if needed.
 		 */
		if (first_cat_eat == true) {
			if (another_cat_eat == true) {
				P(cats_done); /*wait for another cat */

			}
			kprintf("<<< %s leaves kitchen.\n", catnames[catnumber]);
			no_cat_eat = true; //no cats are eating now

			P(mutex);
			//Switch to mice if any waiting
			if (mice_wait_count > 0) {
			//Wakeup the mice
				V(mice_queue);
			} else if (cats_wait_count > 0) { //if no waiting mice, go to cats
				//Wakeup the cats
				V(cats_queue);	
			} else { //no animals waiting for food
				all_dishes_available = true;
			}
			V(mutex);
		} else { //Non first cat leave
			kprintf("<<< %s leaves kitchen.\n", catnames[catnumber]);
			V(cats_done);
		}
	}

	/*Out of loop, cat done with turns*/
	V(done);
	kprintf("... %s exiting.\n", catnames[catnumber]);
	
}
/*
 *setup()
 *
 *Arguements:
 *	None
 *
 *Returns :
 * 	nothing.
 *
 * Notes:
 * 	Initialize semaphores and values, will most likely get rid of value declarations if 
 * 	it causes no errors, as they are already defined.
 */
static void setup() {
	//create semaphores, MIGHT NEED TO ADD PANIC STATEMENTS FROM LECTURE. CHECK
	mutex = sem_create("mutex", 1);
	dish_mutex = sem_create("dish_mutex", 1);
	done = sem_create("done", 0);
	mice_queue = sem_create("mice_queue", 0);
	cats_queue = sem_create("cats_queue", 0);
	cats_done = sem_create("cats_done", 0);
	mice_done = sem_create("mice_done", 0);
	//Initialize to values, able to get rid of this bc of previous declaration
	all_dishes_available = true; 
        cats_wait_count = 0;
        mice_wait_count = 0;
	no_cat_eat = true;
	no_mouse_eat = true;

}
/*
 * cleanup()
 *
 * Arguements:
 * 	None
 *
 * Returns:
 * 	nothing.
 *
 * Notes:
 *	Destroys all of the semaphores.
 */
static void cleanup() {
	sem_destroy(done);
	sem_destroy(mutex);
	sem_destroy(dish_mutex);
	sem_destroy(mice_queue);
	sem_destroy(cats_queue);
	sem_destroy(cats_done);
	sem_destroy(mice_done);
	done = NULL;
	mutex = NULL;
	dish_mutex = NULL;
	mice_queue = NULL;
	cats_queue = NULL;
	cats_done = NULL;
	mice_done = NULL;
}        
/*
 * mousesem()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using semaphores.
 *      Similar to catsem(), the print statements in this function
 *      are modeled after the print statements in catsem.script
 *      in order to visualize if the program is running correctly.
 */

static
void
mousesem(void * unusedpointer, 
         unsigned long mousenumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) unusedpointer;
        (void) mousenumber; //MIGHT BE ABLE TO GET RID OF THIS

	/*Print mouse is starting*/
	kprintf("... %s starting.\n", mousenames[mousenumber]);

	/*loop to control amount of times eating*/
	int i;
	for (i = 0; i < NEAT; i++) {
		bool first_mouse_eat = false;
		bool another_mouse_eat = false;
		int mydish = 0;

		/*Set max eat/play time for mouse*/
		//clocksleep(random() % MTIME);

		/*Print mouse is hungry*/
		kprintf("    %s feels hungry.\n", mousenames[mousenumber]);

		/*
 		 *  Allow the first mouse into the kitchen while no cat
 		 *  is present in the kitchen.
 		 */
		P(mutex);
		if(all_dishes_available == true) { //Allow mouse in while no cat is there
			all_dishes_available = false; 
			V(mice_queue); //First mouse allowed in
		}
		mice_wait_count++;
		V(mutex);
		P(mice_queue);
		
		/*
 		 *Check if a mouse is eating yet, if not, mouse is first mouse
 		 */
		if(no_mouse_eat == true) {
			no_mouse_eat = false;
			first_mouse_eat = true;
		} else { 
			//another mouse was eating first
			first_mouse_eat = false;
		}
		/*
 		 *Make the first mouse take control of the kitchen
 		 */
		if(first_mouse_eat == true) {
			P(mutex);
			if(mice_wait_count > 1) {
				another_mouse_eat = true;
				V(mice_queue);
			}
			V(mutex);
		}

		/*Print that the mouse is entering the kitchen*/
		kprintf(">>> %s enters kitchen.\n", mousenames[mousenumber]);

		/*
 		 * Assign dishes to mouse
 		 */
		P(dish_mutex);
		if(dish1_busy == false) {
			dish1_busy = true;
			mydish = 1; //Assign dish 1 to mouse
		} else {
			assert(dish2_busy == false); //Make sure dish 2 isnt busy
			dish2_busy = true;
			mydish = 2; //assign dish 2 to mouse
		}
		V(dish_mutex);
		/*
 		 *Allow mouse to eat
 		 */
		//Print that the mouse is begining to eat
		kprintf("*** %s starts eating at dish %d.\n", mousenames[mousenumber], mydish);
		clocksleep(random() % MTIME); //enjoy food
		kprintf("*** %s done eating at dish %d.\n", mousenames[mousenumber], mydish);


		/*
 		 *Mouse is done eating, now we must release the dish
 		 */
		 P(mutex);
		 P(dish_mutex);
		 if(mydish == 1) {
			dish1_busy = false; //release dish 1
			//Print mouse is releasing the dish
			kprintf("*** %s is releasing dish %d.\n", mousenames[mousenumber], mydish);
		 } else {
			assert(mydish == 2); //make sure it holds dish 2
			dish2_busy = false; //release dish 2
			//print mouse is releasing the dish
			kprintf("*** %s is releasing dish %d.\n", mousenames[mousenumber], mydish);
		 }
		V(dish_mutex);
		mice_wait_count--; //decrement the number of mice waiting
		V(mutex);

		/*
 		 * Mouse has released dish, now they must leave the kitchen
 		 */
		if (first_mouse_eat == true) {
			if(another_mouse_eat == true) {
				P(mice_done);
			}
			//Print that the mouse is leaving the kitchen
			kprintf("<<< %s leaves kitchen.\n", mousenames[mousenumber]);
			no_mouse_eat = true; //no mice are eating anymore
			P(mutex);
			/*
 			 * Now we must switch turns, check if cats are waiting, 
 			 * if there are none, then check if the mice are waiting.
 			 */
			 if (cats_wait_count > 0) {
				V(cats_queue);
		       	} else if(mice_wait_count > 0) {
				V(mice_queue);
			} else { 
				all_dishes_available = true; //this means no animals are waiting
			}
			V(mutex);
		} else {
			//print that non first mouse is leaving
			kprintf("<<< %s leaves kitchen.\n", mousenames[mousenumber]);
			V(mice_done);
		}
	} //End bracket for for loop, comment to make debugging easier
	/*
         * Loop has finished, mice are done
         */	
	 V(done);
	 //Print that mouse is exiting
	 kprintf("... %s exiting.\n", mousenames[mousenumber]);
}


/*
 * catmousesem()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catsem() and mousesem() threads.  Change this 
 *      code as necessary for your solution.
 */

int
catmousesem(int nargs,
            char ** args)
{
        int index, error;
   
        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;
   
		
	/*
   	 *Call setup()
	 */
	setup();
 
       /*
         * Start NCATS catsem() threads.
         */

        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catsem Thread", 
                                    NULL, 
                                    index, 
                                    catsem, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
                 
                        panic("catsem: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }
        
        /*
         * Start NMICE mousesem() threads.
         */

        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mousesem Thread", 
                                    NULL, 
                                    index, 
                                    mousesem, 
                                    NULL
                                    );
                
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mousesem: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

	//Wait for threads to finish
	int i = 0;
	while (i < NANIMALS) {
		P(done);
		i++;
	}
	//Call cleanup()
	cleanup();
        return 0;
}


/*
 * End of catsem.c
 */
