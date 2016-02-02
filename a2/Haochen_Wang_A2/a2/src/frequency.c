/*
 ============================================================================
 Name        : freqiency.c
 Author      : Haochen Wang
 Version     : COMP428 Assignment 2
 Copyright   : Your copyright notice
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char* argv[]){
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	int source;   /* rank of sender */
	int dest;     /* rank of receiver */
	int tag=0;    /* tag for messages */
	char message[100];        /* storage for message */
	MPI_Status status ;   /* return status for receive */
	int stringLength = 128;
	char** as; // 2-D array to get the file
	char** split;
	int n;
	int i;

	/* start up MPI */
	MPI_Init(&argc, &argv);
	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p); 


	if(my_rank == 0){
		//Open a "input.txt" file
		char* textNameArray="input.txt";
		FILE *file;
		file = fopen(textNameArray, "r");

		//To get the file size
		fseek(file, 0, SEEK_END);
		int fileSize;
		fileSize = ftell(file);
		fclose(file);
		file == NULL;

		//To get the size of each processes to get
		n = (fileSize/(p * stringLength))+1;
		as = malloc(sizeof(char*)*n);

		for(i=0;i<n;i++){
			as[i] = malloc(sizeof(char)*(p * stringLength));
		}

		//Open file again,then read it, and put in 2-D array
		file = fopen(textNameArray, "r");
		if(file==NULL){
			printf("No file!\n");
			printf("\n");
		}
		else{
			int count=0;
			while(fread(as[count], (p*stringLength), sizeof(char), file)!=NULL){
				count++;
			}
		}
	}


	if(my_rank == 0){
		printf("Partial Totals:\n");
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	split = malloc(sizeof(char*)*n);
	for(i=0;i<n;i++){
		split[i] = malloc(sizeof(char)*(stringLength));
	}

	//Separate file to 5 processes
	for (i = 0; i < n; ++i){
		MPI_Scatter(as[i], stringLength, MPI_CHAR, split[i], stringLength, MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	
	
	/*
	//printf("process %d\n", my_rank);
	for (i = 0; i < n; ++i){
		//printf("process %d ::: %s\n", my_rank, split[i]);

	}
	*/

	
	//To calculate the total number of a-z in each process
	char str[26];
	int j,k,l, a[26]={0};

	for(j = 0; j < 26; ++j){
		str[j] = 65+j;
	}

	for(i = 0; i < n; ++i){
		for(k = 0; k < 128; ++k){
			for(l=0;l<26;++l){
				if(split[i][k]==str[l]||(split[i][k]==str[l]+32)){
					a[l]++;
				}
			}
		}
	}


	//print processes one by one from process0 to process4
	for(i=0;i<p;i++){
	
		if(my_rank==i){
			printf("process %d\n",my_rank);
			for(j=0;j<26;j++){

                		printf("%c\t%d\n",str[j],a[j]);
		        }

		}
		MPI_Barrier(MPI_COMM_WORLD);

	}


	/*
	printf("process %d\n", my_rank);
	for(j=0;j<26;j++){

		printf("process %d\t%c\t%d\n",my_rank,str[j],a[j]);
	}
	*/

	
	//To get sum of a-z in all processes
	int s;
	int global_sum[26]={0};

	for(s=0;s<26;++s){
		MPI_Reduce(&a[s],&global_sum[s],1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	}	

	
	//separate before part, start the partial total 
	MPI_Barrier(MPI_COMM_WORLD);

	if(my_rank == 0){

		printf("Process 0 Partial total\n");
		for(j=0;j<26;j++){
				printf("%c\t%d\n",str[j],global_sum[j]);
			}


	}

	
	//using '*' to replace the total number of a-z
	if(my_rank == 0){
		for(i=0;i<26;i++){
				printf("%c",str[i]);
				for(j=0;j<global_sum[i];j++){
					printf("%c",'*');
				}
				printf("\n");
			}


	}
	
	
	MPI_Finalize(); 
	
	
	return 0;
}
