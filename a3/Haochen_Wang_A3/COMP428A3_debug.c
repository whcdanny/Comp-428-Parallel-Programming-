/*
 ============================================================================
 Name        : COMP428A3_debug.c

 Description :

    Compute a matrix-vector product b = A * x. Each worker is given
    a copy of x, and then is fed one row of A.  As soon as it computes
    b(I) = A(I,1:N)*x(1:N), it is given another column of A, unless
    there are no more, in which case it is sent a "terminate" message.
    Thus, a faster process will be given more work to do. The master process
    allocates A and x, but the worker processes only allocate enough memory
    for one row of A, and x.
 ============================================================================
 */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>

# include "mpi.h"


int main ( int argc, char *argv[] ) {
	double *a;
	double *a_row;
	double ans;
	double *b;
	int dest;
	int dummy;
	int i, j, k, m, n;
	int j_one;
	int master = 0;
	int my_id;
	int num_procs;
	int num_rows;
	int num_workers;
	double pi = 3.141592653589793;
	MPI_Status status;
	int tag;
	int tag_done;
	double *x;


	MPI_Init ( &argc, &argv );
	MPI_Comm_rank ( MPI_COMM_WORLD, &my_id );
	MPI_Comm_size ( MPI_COMM_WORLD, &num_procs );


	// Message from root process
	if ( my_id == 0 ){
		printf ( "\n" );
		printf ( "Master process:\n" );
		printf ( "  Computing matrix-vector product b = A * x.\n" );
		printf ( "  The number of processes is %d.\n", num_procs );
	}

	// All processes
	printf ( "\n" );
	printf ( "  ++ Process %d is active ++\n", my_id );

	m = 20;
	n = 10;
	tag_done = m + 1;

	if ( my_id == 0 ) {
		printf ( "\n" );
		printf ( "  The number of rows is    %d.\n", m );
		printf ( "  The number of columns is %d.\n", n );
	}



	/*
	 * The master process allocates and initializes A and X.
	 * Because we are dynamically allocating A, we can't use 2D array double
	 * indexing, so we have to figure out where we are on our own.
	 */

	if ( my_id == master ){
		a = ( double * ) malloc ( m * n * sizeof ( double ) );
		x = ( double * ) malloc ( n * sizeof ( double ) );
		b = ( double * ) malloc ( m * sizeof ( double ) );


		/* This is the place where the value of the matrix and vector are
		 * initialized. There are NO errors  or bugs in this section.
		 *
		 *
		 * X is specially chosen so that b = A * x is known in advance.
		 * The value of b will be zero, except that entry J_ONE will be 1.
		 * Pick any value of J_ONE between 1 and M.
		 */

		k = 0;
		for ( i = 1; i <= m; i++ ) {
			for ( j = 1; j <= n; j++ ){
				a[k] = sqrt ( 2.0 / ( double ) ( n + 1 ) )
        		  * sin ( ( double ) ( i * j ) * pi / ( double ) ( n + 1 ) );
				k = k + 1;
			}
		}

		j_one = 17;
		for ( i = 0; i < n; i++ ) {
			x[i] = sqrt ( 2.0 / ( double ) ( n + 1 ) )
        		* sin ( ( double ) ( ( i + 1 ) * j_one ) * pi / ( double ) ( n + 1 ) );
		}

		/*
		 * END of section where there is a guarantee that no errors have occurred
		 * */



		printf ( "\n" );
		printf ( "Master process:\n" );
		printf ( "  Vector x:\n" );
		printf ( "\n" );
		for ( i = 0; i < n; i++ ) {
			printf ( "%d %f\n", i, x[i] );
		}
	}



	/* Worker processes set aside room for one row of A, and for the vector X. */
	else {

		a_row = ( double * ) malloc ( n * sizeof ( double ) );
		x = ( double * ) malloc ( n * sizeof ( double ) );
	}


	/* Process 0 broadcasts the vector X to the other processes. */
	MPI_Bcast ( x, n, MPI_DOUBLE, master, MPI_COMM_WORLD );


	if ( my_id == master ) {
		/*
		 * Process 0 sends one row of A to all the other processes.
		 * If we were using standard C 2D array storage, the entries of
		 * the row would be contiguous; using pointers, we have ended up
		 * in the same situation.  As long as the entries are contiguous,
		 * we can use a simple standard datatype with MPI_Send.
		 * The situation would require a little more work if we tried
		 * to send a column of data instead of a row.
		 */

		num_rows = 0;

		for ( i = 1; i <= num_procs-1; i++ ) {
			dest = i;
			tag = num_rows;
			k = num_rows * n;

			MPI_Send ( a+k, n, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD );

			num_rows = num_rows + 1;
		}

		num_workers = num_procs-1;


		for ( ; ; ){
			MPI_Recv ( &ans, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
					MPI_ANY_TAG, MPI_COMM_WORLD, &status );

			tag = status.MPI_TAG;
			b[tag] = ans;

			if ( num_rows < m ){
				num_rows = num_rows + 1;
				dest = status.MPI_SOURCE;
				tag = num_rows;
				k = num_rows * n;

				MPI_Send ( a+k, n, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD );
			}
			else {
				num_workers = num_workers - 1;
				dummy = 0;
				dest = status.MPI_SOURCE;
				tag = tag_done;

				MPI_Send ( &dummy, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );

				if ( num_workers == 0 )
				{
					break;
				}
			}
		}

		free ( a );
		free ( x );
	}


	/* Each worker process repeatedly receives rows of A (with TAG indicating
	 * which row it is), computes dot products A(I,1:N) * X(1:N) and returns
	 * the result (and TAG), until receiving the "DONE" message.
	 */
	else {
		for ( ; ; ) {
			MPI_Recv ( a_row, n, MPI_DOUBLE, master, MPI_ANY_TAG,
					MPI_COMM_WORLD, &status );

			tag = status.MPI_TAG;

			if ( tag == tag_done ){
				printf ( "  -- Process %d shutting down --\n", my_id );
				break;
			}

			ans = 0.0;
			for ( i = 0; i < n; i++ ){
				ans = ans + a_row[i] * x[i];
			}

			MPI_Send ( &ans, 1, MPI_DOUBLE, master, tag, MPI_COMM_WORLD );
		}

		free ( a_row );
		free ( x );
	}


	/* Print out the answer.*/
	if ( my_id == master ) {
		printf ( "\n" );
		printf ( "Master process:\n" );
		printf ( "  Product vector b = A * x\n" );
		printf ( "  (Should be zero, except for a 1 in entry %d)\n", j_one-1 );
		printf ( "\n" );
		for ( i = 0; i < m; i++ ) {
			printf ( "%d %f\n", i, b[i] );
		}

		free ( b );
	}



	MPI_Finalize ( );

	if ( my_id == master ) {
		printf ( "\n" );
		printf ( "Master process:\n" );
		printf ( "  Normal end of execution.\n" );
		printf ( "\n" );
	}
	return 0;
}

