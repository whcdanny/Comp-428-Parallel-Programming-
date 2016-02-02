

This programming is working.

I used th school computer, and i did not separate the db.c file to menu.c,even.c and odd.c. But i ask for professor it only will loss 3 or 4 mark, so sorry about it, I really do not have time to finish it. 
And i using :: mpicc \-o db db.c   AND   mpirun -np 10 db 
IT IS WORKING 

So i only have a db.c file to run it.

1. create 4 structs for my project, and then datatype them

2. creat different 3 communicator : all processes, Even and Odd processes, And Node processes(only have 0 and 1)

3. using timer to read text file and save to a table:void sigFunc(){}

4. Create a menu

5. let start

	1. Get the read_max

	2. go first menu
	
	3. Depent your choice 1 or 2 or 3
	
	4. Even processes send your choice to Odd processes

	5. Then, Odd processes if did not receive, it read text file using timer, if receive, depend on your choice to do something. Finally, it send the table to Even processes, here i sort the data, because it is easy to Even to do.

	6. Now, Even processes receive the Odd's table and  Bucket sort, and send final answer to the Root(process 0)

	7. finally, the Root(process 0) get final answer, and print it in screen


The more datils in the project code.

Thank you! 
