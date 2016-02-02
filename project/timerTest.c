/*
 ============================================================================
 Name        : timerTest.c

 Description : Simple timer test. The code runs in a huge loop and simply
  calculates a meaningless operation. As this happens, an Alarm will be fired
  every 3 seconds. The OS will interrupt the process and run the signal handler
  function when this happens. The signal handler will print a message and
  then reset the alarm. Control will then return to where the program had
  been running before the interrupt.

  Note: you probably don't want to wait until the program actually finishes
  since the loop is very large. So you will want to just kill this
  process after a few seconds.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>



/* we make some variables global so they can be seen easily from
   the signal handler and the main program */

time_t start_time;
int i,j,k;
double ans=0.0;



/* Our signal handler.  We will tell the system to call this
   function whenever it gets a SIGALRM */
void sigFunc(int sig) {

	printf("Alarm!  Alarm! Received signal %d at time %d\n", sig, (int)(time(NULL) - start_time));
	printf("Main program is currently in loop iteration: %d\n\n", i * j * k);

	/* reset the timer so we get called again in 5 seconds */
	alarm(3);
}




int main() {

	/* starting time - this returns the number of seconds
     since the Unix epoch at 1970/01/01 00:00:00 */
	start_time = time(NULL);

	printf("This program will compute for a long time, but\n");
	printf("will handle SIGALRM events asynchronously to\n");
	printf("report on its progress every 3 seconds\n\n");

	/* set up our signal handler to catch SIGALRM */
	signal(SIGALRM, sigFunc);

	/* start the timer - we want to wake up in 5 seconds */
	alarm(3);

	/* compute stuff for a long time */
	for (i=0;i<10000;i++)
		for (j=0;j<10000;j++)
			for(k=0;k<10000;k++)
				ans=ans+sqrt(i)+sqrt(j)-sqrt(k); // a time consuming calculation


	/* You can just kill the program instead of waiting to finish */
	return 0;
}

