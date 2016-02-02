This programming is working.

a3.c 

1. Create a structure include date, location, and data
	struct Rainfall{}
2. Create a method to Compare the two amounts
	int compareAmount(const void *a,const void *b){}

3. Create variables for this programming.
	int  my_rank; /* rank of process */
    
	int  p;       /* number of processes */
    
	int source;   /* rank of sender */
    
	int dest;     /* rank of receiver */
    
	int tag=0;    /* tag for messages */
    
	MPI_Status status ;   /* return status for receive */
    
	const int EVENT=10000;/*number of Event*/
    
	const int n=12000;/*Receive buffer size*/
    
	const float AMOUNTMAX = 1000.0;/*Max Rang of amount*/
    
	int i,j,k,l;
   
    
	struct Rainfall *rcvBuffer;/*1st receive buffer of Rainfall*/
    		
	struct Rainfall *rcvBuffer1;/*2nd receive buffer of Rainfall */
    		
	struct Rainfall *merge_array;/*merge buffer of Rainfall*/
    
	struct Rainfall *final_array;/*final buffer of Rainfall*/
    
	int spointer=0;/*pointer of in 1st time send Rainfall array*/
    
	int spointer1=0;/*pointer of in 2nd time send Rainfall array*/
    		
	int rpointer=0;/*pointer of in 1st time receive Rainfall array*/
    
	int rpointer1=0;/*pointer of in 2nd time receive Rainfall array*/

4. Start up MPI 
	a. Create my structure to a MPI Datatype  MPI_Rain
	b. First send and receive rainfall event
		i. random data(year, month, day, latitude, longitude,day)
		ii. sort Rainfall events eventArray 
		iii. Get 1/p of its rainfall events number in each process
		iv. 1st send and receive in all processes
		v. sort Rainfall events rcvBuffer 
	c.Second send and receive rainfall event
		i. random data(year, month, day, latitude, longitude,day)
		ii. sort Rainfall events eventArray1 
		iii. Get 1/p of its rainfall events number in each process
		iv. 2nd send and receive in all processes
		v. sort Rainfall events rcvBuffer 1
	d. Use merge to sort rcvBuffer and rcvBuffer1 data in merge_array
		i. Compare rcvBuffer amount and rcvBuffer1 amount
		ii. Send small amount in merge_array
	e. Finaly, Send merge_array of each process in final_array of processes1

5. End of MPI