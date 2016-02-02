

This programming is working.

Frequency.c

1. Create variables for this programming.
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	MPI_Status status ;   /* return status for receive */
	int stringLength = 128;/*the string length*/
	char** as; /*2-D array to get all the file information*/
	char** split;/*each process 2-D array*/

2. Start up MPI 
	a. Open a "input.txt" file, load it into memory.
	b. Get the file size, then calculate how much size get for each 	   	   process. 
	c. When i get the size of each process, open file again, and put in 	   'as'(2-D array) and 'split'(each process 2-D array).
	d. Using "MPI_Scatter()" separate all into 5 processes, then to 	   calculate the total number of a-z in each process, and print it.
	e. In order to the sequence, using "MPI_Barrier(MPI_COMM_WORLD)", 	   print processes one by one from process0 to process4
	f. Using "MPI_Reduce" get the sum of a-z in all processes, and print 	   it.
	g. Using '*' to replace the total number of a-z, print it.

3. Finish MPI

	