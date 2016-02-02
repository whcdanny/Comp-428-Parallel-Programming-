/*
 ============================================================================
 Name        : test.c
 Author      : Haochen Wang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include "mpi.h"

//Structure  of data info
struct Db{
	unsigned short int sales_id;
	unsigned short int year;
	unsigned short int month;
	unsigned short int day;
	unsigned short int company_id;
	char company_name[100];
	double sales_total;
};

//Structure of Choice 
struct Choice{
	int number;
	unsigned short int startyear;
	unsigned short int startmonth;
	unsigned short int startday;
	unsigned short int finalyear;
	unsigned short int finalmonth;
	unsigned short int finalday;
};

//Structure of Choice 1 print
struct FinalPrint{
	unsigned short int company_id;
	char company_name[100];
	double sales_total;
};

//Structure of Choice 2 print
struct TimePrint{
	unsigned short int year;
	unsigned short int month;
	unsigned short int day;
	double sales_total;
};



//Compare of company id of Structure data info 
int compareCompanyId(const void *a,const void *b){
	
	struct Db *r1=(struct Db *)a;
	struct Db *r2=(struct Db *)b;

	if(r1->company_id>r2->company_id){
		return 1;
	}
	else if(r1->company_id<r2->company_id){
		return -1;
	}
	else
		return 0;
}

//Compare of company id of Structure of Choice 1 data info 
int compareFinalCompanyId(const void *a,const void *b){
	
	struct FinalPrint *r1=(struct FinalPrint *)a;
	struct FinalPrint *r2=(struct FinalPrint *)b;

	if(r1->company_id>r2->company_id){
		return 1;
	}
	else if(r1->company_id<r2->company_id){
		return -1;
	}
	else
		return 0;
}

//Compare date of Structure data info 
int compareTime(const void *a,const void *b){
	struct Db *r1=(struct Db *)a;
	struct Db *r2=(struct Db *)b;

	if(r1->year>r2->year){
		return 1;
	}
	else if(r1->year<r2->year){
		return -1;
	}
	else if(r1->year==r2->year){
		if(r1->month>r2->month){
			return 1;
		}
		else if(r1->month<r2->month){
			return -1;		
		}
		else if(r1->month==r2->month){
			if(r1->day>r2->day){
				return 1;
			}
			else if(r1->day<r2->day){
				return -1;
			}
			else{
				return 0;
			}
		}
		else{
			return 0;	
		}
	}
	else{
		return 0;
	}
		
}	

//Let the date to translate to the number, because it is easy to separate sort 
int caculateTime(unsigned short int a,unsigned short int b, unsigned short int c){
	int timesum=0;
	timesum=a*365+b*31+c;
	return timesum;
	

}

//Menu 
void menu(struct Choice* csendBuffer, int *test){
		//*csendBuffer = (struct Choice*)malloc(1*sizeof(struct Choice));
		int syear;
		int smonth;
		int sday;
		int eyear;
		int emonth;
		int eday;
		int choice1;
		printf("Parallel DBMS Main Menu\n\n");
		printf("1. Company Sales\n");
		printf("2. Sales by Date\n");
		printf("3. Exit\n");
		
		scanf("%i",&choice1);

		if(choice1==1){
			csendBuffer[0].number=1;
			csendBuffer[0].startyear=0;
			csendBuffer[0].startmonth=0;
			csendBuffer[0].startday=0;
			csendBuffer[0].finalyear=0;
			csendBuffer[0].finalmonth=0;
			csendBuffer[0].finalday=0;
		}
		else if(choice1==2){
			printf("Enter Starting Year:\n");
			scanf("%d",&syear);
			printf("Enter Starting Month:\n");
			scanf("%d",&smonth);
			printf("Enter Starting Day:\n");
			scanf("%d",&sday);
			printf("Enter Ending Year:\n");
			scanf("%d",&eyear);
			printf("Enter Ending Month:\n");
			scanf("%d",&emonth);
			printf("Enter Ending Day:\n");
			scanf("%d",&eday);
			csendBuffer[0].number=2;
			csendBuffer[0].startyear=syear;
			csendBuffer[0].startmonth=smonth;
			csendBuffer[0].startday=sday;
			csendBuffer[0].finalyear=eyear;
			csendBuffer[0].finalmonth=emonth;
			csendBuffer[0].finalday=eday;		
		}
		else if(choice1==3){
			csendBuffer[0].number=3;
		}

}
struct Db db; 
//struct Choice c;

int numrecord=0;
int test_1=1;
int recvflag=0;
int totalTablesum=0;
int recvtotalTablesum=0;
int test=1;

//Timer
void sigFunc(int sig){
	test_1=1;
	/* reset the timer so we get called again in 3 seconds */		
	alarm(3);
}


int main(int argc, char* argv[]){
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	int  eo_rank; /* rank of even odd of process */
	int  node_rank;/* rank of node of process */
	int eo_p;	 /* even odd number of processes */
	int  node_p;       /* node number of processes */
	int source;   /* rank of sender */
	int dest;     /* rank of receiver */
	int tag=0;    /* tag for messages */

				
	MPI_Status status ;   /* return status for receive */
	MPI_Request request; /*return request status for receive*/
	
	int n=50000;
	int try=60000;
	unsigned short int max_company_id;
	int datacount=0;
	
	int i,j,k,l;
	int finsum2=0;
	int getnum2=0;
	int read_max=0;
	int color1,key1,color2,key2;

	struct Choice *csendBuffer;
	//struct Choice *crecvBuffer;

	srand(time(NULL) *my_rank);
	int t=rand()%3+2;
		

	
	
	

	//Even and Odd processes
	MPI_Comm EO_Comm;
	//Node process 0 and process 1
	MPI_Comm Node_Comm;

	/* start up MPI */
	MPI_Init(&argc, &argv);
	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p); 

//=================================================Build Datatype=======================================================================

	//Create MPI Datatype  MPI_Choice
	unsigned int typeCount1;
	MPI_Datatype MPI_Choice;
	MPI_Datatype types1[7]={MPI_INT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT};
	MPI_Aint offset1[7];
	int blockLength1[7]={1,1,1,1,1,1,1};
	offset1[0]=offsetof(struct Choice,number);
	offset1[1]=offsetof(struct Choice, startyear);
	offset1[2]=offsetof(struct Choice, startmonth);
	offset1[3]=offsetof(struct Choice, startday);
	offset1[4]=offsetof(struct Choice, finalyear);
	offset1[5]=offsetof(struct Choice, finalmonth);
	offset1[6]=offsetof(struct Choice, finalday);
	MPI_Type_create_struct(7,blockLength1,offset1,types1,&MPI_Choice);
	MPI_Type_commit(&MPI_Choice);

	//Create MPI Datatype  MPI_Db
	unsigned int typeCount2;
	MPI_Datatype MPI_Db;
	MPI_Datatype types2[7]={MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_CHAR,MPI_DOUBLE};
	MPI_Aint offset2[7];
	int blockLength2[7]={1,1,1,1,1,64,1};
	offset2[0]=offsetof(struct Db,sales_id);
	offset2[1]=offsetof(struct Db, year);
	offset2[2]=offsetof(struct Db, month);
	offset2[3]=offsetof(struct Db, day);
	offset2[4]=offsetof(struct Db,company_id);
	offset2[5]=offsetof(struct Db, company_name);
	offset2[6]=offsetof(struct Db, sales_total);
	MPI_Type_create_struct(7,blockLength2,offset2,types2,&MPI_Db);
	MPI_Type_commit(&MPI_Db);

	//Create MPI Datatype  MPI_FinalPrint
	unsigned int typeCount3;
	MPI_Datatype MPI_FinalPrint;
	MPI_Datatype types3[3]={MPI_UNSIGNED_SHORT,MPI_CHAR,MPI_DOUBLE};
	MPI_Aint offset3[3];
	int blockLength3[3]={1,64,1};
	offset3[0]=offsetof(struct FinalPrint,company_id);
	offset3[1]=offsetof(struct FinalPrint, company_name);
	offset3[2]=offsetof(struct FinalPrint, sales_total);
	MPI_Type_create_struct(3,blockLength3,offset3,types3,&MPI_FinalPrint);
	MPI_Type_commit(&MPI_FinalPrint);

	//Create MPI Datatype  MPI_TimePrint
	unsigned int typeCount4;
	MPI_Datatype MPI_TimePrint;
	MPI_Datatype types4[4]={MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT,MPI_DOUBLE};
	MPI_Aint offset4[4];
	int blockLength4[4]={1,1,1,1};
	offset4[0]=offsetof(struct TimePrint, year);
	offset4[1]=offsetof(struct TimePrint, month);
	offset4[2]=offsetof(struct TimePrint, day);
	offset4[3]=offsetof(struct TimePrint, sales_total);
	MPI_Type_create_struct(4,blockLength4,offset4,types4,&MPI_TimePrint);
	MPI_Type_commit(&MPI_TimePrint);

	//Create Even and Odd communicator 
	color1 = my_rank%2;
	key1 = my_rank/2;
	MPI_Comm_split(MPI_COMM_WORLD, color1, key1,&EO_Comm);
	/* find out Even and Odd process rank */
	MPI_Comm_rank(EO_Comm, &eo_rank);
	/* find out Even and Odd number of processes */
	MPI_Comm_size(EO_Comm, &eo_p); 
	
	//Create Node communicator 
	color2 = my_rank/2;
	key2 = my_rank%2;
	MPI_Comm_split(MPI_COMM_WORLD, color2, key2,&Node_Comm);
	/* find out Node process rank */
	MPI_Comm_rank(Node_Comm, &node_rank);
	/* find out Node number of processes */
	MPI_Comm_size(Node_Comm, &node_p); 
	

	//csendBuffer = (struct Choice*)malloc(1*sizeof(struct Choice));
	
	
	
//=======================================================Start=================================================================================
	//Get the read_max
	if(my_rank==0){
		printf("Please input a read_max(between 1000 to 100000)\n");
		scanf("%i",&read_max);
		if(read_max<1000&&read_max>100000){
			printf("Not true! Please try again input a read_max(between 1000 to 100000)\n");
			scanf("%i",&read_max);
		}
	}

	while(test==1){

		struct Choice *crecvBuffer = (struct Choice*)malloc(1*sizeof(struct Choice));
		csendBuffer = (struct Choice*)malloc(1*sizeof(struct Choice));
		//====================================================Process 0==============================================================================
	
		if(my_rank==0){
	
			menu(csendBuffer,&test);
		}
	
		MPI_Barrier(MPI_COMM_WORLD);

		MPI_Bcast(&read_max, 1, MPI_INT, 0, MPI_COMM_WORLD);

		MPI_Barrier(MPI_COMM_WORLD);
		//Send choice buffer to all processes
		MPI_Bcast(csendBuffer, 1, MPI_Choice, 0, EO_Comm);

		MPI_Barrier(MPI_COMM_WORLD);
	
	
	
		//=================================================Node of processes 0 (processes 0,2,4,6,8))======================================================
		if(node_rank==0){
			
			MPI_Isend(csendBuffer,1,MPI_Choice,1,0,Node_Comm,&request);
			
			//=====================================If Choice 1: Company Id=================================================================
			if(csendBuffer[0].number==1){
				
				//Receive from Node of processes 1 
				MPI_Recv(&recvtotalTablesum,1,MPI_INT,1,0,Node_Comm,&status);
				//Receive DB 
				struct Db *RDb1=(struct Db*)malloc(recvtotalTablesum*sizeof(struct Db));
				MPI_Recv(&RDb1[0],recvtotalTablesum,MPI_Db,1,0,Node_Comm,&status);

				//All to all receive max of company id
				MPI_Allreduce(&RDb1[recvtotalTablesum-1].company_id,&max_company_id,1,MPI_INT,MPI_MAX,EO_Comm);
			
				//Bucket sort Company Sales
				int *intervalCounter = malloc(eo_p*sizeof(int));
				int interval = max_company_id/eo_p;
				int interval_modulo = max_company_id%eo_p;
				for(i=0;i<eo_p;i++){
					intervalCounter[i]=0;
					for(j=0;j<recvtotalTablesum;j++){
						if(RDb1[j].company_id>(i*interval)&&RDb1[j].company_id<=(i*interval+interval)){
							intervalCounter[i]++;
						}
						else if(i==(eo_p-1)&&RDb1[j].company_id>=(i*interval+interval)){
							intervalCounter[i]++;
						}
					}
					//printf("process %i count form %i to %i, there are %i events\n", my_rank, i*interval, i*interval+interval, intervalCounter[i]);
				}
				

				struct Db *reRDb1=(struct Db*)malloc(try*sizeof(struct Db));
				//Send and receive between Even processes
				int spointer=0;
				int rpointer=0;
				int num=0;
				for(i=0;i<eo_p;i++){
					for(j=0;j<eo_p;j++){
						if(i!=j){
							if(i==eo_rank){
								MPI_Send(&intervalCounter[j],1,MPI_INT,j,0,EO_Comm);
								MPI_Send(&RDb1[spointer],intervalCounter[j],MPI_Db,j,0,EO_Comm);
								spointer=spointer+intervalCounter[j];
								//printf("1process%i==sp==%i\n",my_rank,spointer);	
							}
							else if(j==eo_rank){
								MPI_Recv(&num,1,MPI_INT,i,0,EO_Comm,&status);
								MPI_Recv(&reRDb1[rpointer],num,MPI_Db,i,0,EO_Comm,&status);
								rpointer=rpointer+num;
								//printf("1process%i==rp==%i\n",my_rank,rpointer);
							}
							else{
								continue;
							}
						}
						else {
							if(i==eo_rank&&j==eo_rank){
								for(l=spointer;l<spointer+intervalCounter[eo_rank];l++){
									reRDb1[rpointer].sales_id=RDb1[l].sales_id;
									reRDb1[rpointer].year=RDb1[l].year;
									reRDb1[rpointer].month=RDb1[l].month;
									reRDb1[rpointer].day=RDb1[l].day;
									reRDb1[rpointer].company_id=RDb1[l].company_id;
									memcpy(reRDb1[rpointer].company_name,RDb1[l].company_name,sizeof(char) * 64);
									reRDb1[rpointer].sales_total=RDb1[l].sales_total;
									rpointer=rpointer+1;
								}
								spointer=spointer+intervalCounter[eo_rank];
								//printf("2process%i==sp==%i\n",my_rank,spointer);
								//printf("2process%i==rp==%i\n",my_rank,rpointer);
							}
							else{
								continue;
							}				
						}
					}				
				}
				//Let empty part of company id to more bigger
				for(i=0;i<try;i++){
					if(reRDb1[i].company_id==0){
						reRDb1[i].company_id=1000;		
					}
					else{continue;}
				}
			
				//sort the company id
				qsort(reRDb1, try, sizeof(struct Db),compareCompanyId);

				//Final print choice 1
				struct FinalPrint *fpDb1=(struct FinalPrint*)malloc((interval+interval_modulo)*sizeof(struct FinalPrint));
			
				//Calculate  the total sales of same company id 
				for(i=0;i<eo_p;i++){
					for(j=0;j<try;j++){
						if(i!=(eo_p-1)){
							for(k=1;k<interval+1;k++){
								if(reRDb1[j].company_id==i*interval+k){
									fpDb1[k-1].company_id=reRDb1[j].company_id;
									memcpy(fpDb1[k-1].company_name,reRDb1[j].company_name,sizeof(char) * 64);
									fpDb1[k-1].sales_total=fpDb1[k-1].sales_total+reRDb1[j].sales_total;					
								}
								else{				
								}
							}						
						}
						else if(i==(eo_p-1)){
							for(k=1;k<interval+interval_modulo+1;k++){
								if(reRDb1[j].company_id==i*interval+k){
									fpDb1[k-1].company_id=reRDb1[j].company_id;
									memcpy(fpDb1[k-1].company_name,reRDb1[j].company_name,sizeof(char) * 64);
									fpDb1[k-1].sales_total=fpDb1[k-1].sales_total+reRDb1[j].sales_total;					
								}
								else{													
								}
							}							
						}
						else{continue;}
					}
				}

				//Sort the final print 
				qsort(fpDb1,(interval+interval_modulo), sizeof(struct FinalPrint),compareFinalCompanyId);
				
				//Send final company sales total to process 0
				int sendnumber1 = interval;
				int sendnumber2 = interval+interval_modulo;
				for(i=0;i<eo_p;i++){
					if(eo_rank==i&&i!=eo_p-1){
						MPI_Send(&sendnumber1,1,MPI_INT,0,0,EO_Comm);
						MPI_Send(&fpDb1[0],sendnumber1,MPI_FinalPrint,0,0,EO_Comm);						
					}
					else if(eo_rank==i&&i==eo_p-1){
						MPI_Send(&sendnumber2,1,MPI_INT,0,0,EO_Comm);
						MPI_Send(&fpDb1[0],sendnumber2,MPI_FinalPrint,0,0,EO_Comm);								
					}
			
				}
			}


			//==============================================If Choice 2: Sales by Date===================================================================
		
			else if(csendBuffer[0].number==2){
				
				int recvnum=0;
				//Receive each number of date from Node process 1 
				MPI_Recv(&recvnum,1,MPI_INT,1,0,Node_Comm,&status);
			
				//Receive Date from Node process 1 
				struct Db *DataRDb1=(struct Db*)malloc(recvnum*sizeof(struct Db));
				MPI_Recv(&DataRDb1[0],recvnum,MPI_Db,1,0,Node_Comm,&status);

				//Using the Node 1 process 0 to get the time interval  
				int *timesepsum = malloc((eo_p-1)*sizeof(int));
				int tnum=0;
				if(eo_rank==0){
					int sep=recvnum/eo_p;
					for(i=1;i<eo_p;i++){
						timesepsum[i-1]=caculateTime(DataRDb1[i*sep].year,DataRDb1[i*sep].month,DataRDb1[i*sep].day);	
					}
				}
				//Send the time interval date(year,month,day) to Even processes(process 0,2,4,6,8)
				MPI_Bcast(timesepsum,eo_p-1, MPI_INT, 0, EO_Comm);
			
				//Get time interval count 
				int *timeintervalCounter = malloc(eo_p*sizeof(int));
				for(i=0;i<eo_p;i++){
					timeintervalCounter[i]=0;
					for(j=0;j<recvnum;j++){
						int recvtimesum=caculateTime(DataRDb1[j].year,DataRDb1[j].month,DataRDb1[j].day);
						if(i==0&&recvtimesum<=timesepsum[i]){
							timeintervalCounter[i]++;
						}
						else if(i==(eo_p-1)&&recvtimesum>timesepsum[eo_p-2]){
							timeintervalCounter[i]++;
						}
						else if(recvtimesum>timesepsum[i-1]&&recvtimesum<=timesepsum[i]){
							timeintervalCounter[i]++;
						}
						else{}	
					} 
				
				}

				//Bucket sort Sales by Dates
				struct Db *timeDataRDb1=(struct Db*)malloc(try*sizeof(struct Db));

				//Send and receive between Even processes
				int *spointer2=malloc(sizeof(int));
				*spointer2 = 0;
				int *rpointer2=malloc(sizeof(int));
				*rpointer2 = 0;
				int *num2=malloc(sizeof(int));
				*num2 = 0;
				for(i=0;i<eo_p;i++){
					for(j=0;j<eo_p;j++){
						if(i!=j){
							if(i==eo_rank){
								MPI_Send(&timeintervalCounter[j],1,MPI_INT,j,0,EO_Comm);
								MPI_Send(&DataRDb1[*spointer2],timeintervalCounter[j],MPI_Db,j,0,EO_Comm);
								*spointer2=*spointer2+timeintervalCounter[j];
							}
							else if (j==eo_rank){
								MPI_Recv(num2,1,MPI_INT,i,0,EO_Comm,&status);
								MPI_Recv(&timeDataRDb1[0],*num2,MPI_Db,i,0,EO_Comm,&status);
								*rpointer2=*rpointer2+*num2;
							}						
						}
						else{
							if(i==eo_rank&&j==eo_rank){
								for(k=*spointer2;k<*spointer2+timeintervalCounter[eo_rank];k++){
									timeDataRDb1[*rpointer2].sales_id=DataRDb1[k].sales_id;
									timeDataRDb1[*rpointer2].year=DataRDb1[k].year;
									timeDataRDb1[*rpointer2].month=DataRDb1[k].month;
									timeDataRDb1[*rpointer2].day=DataRDb1[k].day;
									timeDataRDb1[*rpointer2].company_id=DataRDb1[k].company_id;
									memcpy(timeDataRDb1[*rpointer2].company_name,DataRDb1[k].company_name,sizeof(char) * 64);
									timeDataRDb1[*rpointer2].sales_total=DataRDb1[k].sales_total;
									*rpointer2=*rpointer2+1;
								}
								*spointer2=*spointer2+timeintervalCounter[eo_rank];
							}
						}
					}
				}
			
				//Let empty part of company id t0 more bigger
				for(i=0;i<try;i++){
					if(timeDataRDb1[i].year==0){
						timeDataRDb1[i].year=3000;		
					}
					else{continue;}
				}
				//Sort date 
				qsort(timeDataRDb1, try, sizeof(struct Db),compareTime);


				//Get the total number of company sales total
				struct TimePrint *fpTimeDb1=(struct TimePrint*)malloc(try*sizeof(struct TimePrint));
			
				//Calculate the sum of company sales total

				int a=0;
				int timecomp1=0;
				int timecomp2=0;
				for(i=0;i<=recvnum;i++){
					if(i==0){
						fpTimeDb1[a].year=timeDataRDb1[i].year;
						fpTimeDb1[a].month=timeDataRDb1[i].month;
						fpTimeDb1[a].day=timeDataRDb1[i].day;
						fpTimeDb1[a].sales_total=timeDataRDb1[i].sales_total;
					}
					else{
					
						timecomp1=caculateTime(timeDataRDb1[i-1].year,timeDataRDb1[i-1].month,timeDataRDb1[i-1].day);
						timecomp2=caculateTime(timeDataRDb1[i].year,timeDataRDb1[i].month,timeDataRDb1[i].day);
						if(timecomp2==timecomp1){
							fpTimeDb1[a].year=timeDataRDb1[i].year;
							fpTimeDb1[a].month=timeDataRDb1[i].month;
							fpTimeDb1[a].day=timeDataRDb1[i].day;
							fpTimeDb1[a].sales_total=fpTimeDb1[a].sales_total+timeDataRDb1[i].sales_total;
						}
						else if(timecomp2!=timecomp1){
							a = a + 1;
							fpTimeDb1[a].year=timeDataRDb1[i].year;
							fpTimeDb1[a].month=timeDataRDb1[i].month;
							fpTimeDb1[a].day=timeDataRDb1[i].day;
							fpTimeDb1[a].sales_total=fpTimeDb1[a].sales_total+timeDataRDb1[i].sales_total;
						}
						else{};
					}
				}
				
				//Send final company sales total to process 0
				for(i=0;i<eo_p;i++){
					if(eo_rank==i){
						MPI_Send(&a,1,MPI_INT,0,0,EO_Comm);
						MPI_Send(&fpTimeDb1[0],a,MPI_TimePrint,0,0,EO_Comm);
					}
				}
			}
			else if(csendBuffer[0].number=3){
				test=3;
			}	
		}
	
		//=================================Node of processes 1 (processes 1,3,5,7,9))=============================================
	
		else if(node_rank==1){
			srand(time(NULL) *my_rank);
			numrecord=rand()%10+read_max;
			time_t start_time;
			start_time = time(NULL);
			signal(SIGALRM, sigFunc);
			alarm(3);
			totalTablesum=totalTablesum+numrecord;
			//Send DB 
			struct Db *SDb1=(struct Db*)malloc(totalTablesum*sizeof(struct Db));
			recvflag=0;
			//Receive the choice from Node process 0
			MPI_Irecv(crecvBuffer,1,MPI_Choice,0,0,Node_Comm,&request);//no-blocking
			//MPI_Test(&request,&recvflag,&status);
			while(recvflag==0){
				if(test_1==1){
					//printf("start read\n");
					//Read all files, because i do not fix random number to read 
					char textNumber=(char)(my_rank/2+48);
					char textNameArray[256];
					textNameArray[0]='d';
					textNameArray[1]='a';
					textNameArray[2]='t';
					textNameArray[3]='a';
					textNameArray[4]=textNumber;
					textNameArray[5]='.';
					textNameArray[6]='t';
					textNameArray[7]='x';
					textNameArray[8]='t';
					textNameArray[9]='\0';
					FILE *file;
					//printf("%s\n",textNameArray);
					file = fopen(textNameArray,"r");
					if (file==NULL)
					{
						printf("No file!\n");
						printf("\n");
					}
					else{
						char myString[100];
						k=0;
						while(fgets(myString,100,file)!=NULL&&k<totalTablesum){
							int count=0;
							int index1[3];
							int slash_index[2];
							int s = 0;
							i=0;				
							for(j=0;j<strlen(myString);j++){
								if(myString[j]=='|'){
									index1[i]=count;
									i++;	
								}
								if(myString[j]=='/'){
									slash_index[s]=count;
									s++;
								}
								count++;	
							}
							//Separate the info
							SDb1[k].sales_id=strtol(&myString[0], NULL,10);
							SDb1[k].year=strtol(&myString[index1[0] + 1],NULL,10);
							SDb1[k].month=strtol(&myString[slash_index[0] + 1],NULL,10);
							SDb1[k].day=strtol(&myString[slash_index[1] + 1],NULL,10);
							SDb1[k].company_id=strtol(&myString[index1[1] + 1],NULL,10);
							int n1=index1[2];
							int n2=index1[3];
							memcpy(SDb1[k].company_name,myString+n1+1,(n2-n1-1));
							SDb1[k].company_name[n2-n1-1]='\0';
							SDb1[k].sales_total=strtod(myString+n2+1,NULL);
							k++;
							memset(myString, 0, sizeof(myString));
						}
						printf("\n");
					}
					test_1==0;
				}	
				MPI_Test(&request,&recvflag,&status);
			}

			
			//=================================Node of processes 0 (processes 0,2,4,6,8))===================================================

			if(recvflag){
				//===============================If Choice 1: Company Id=================================================================
				if(crecvBuffer[0].number==1){
					//Sort the company Id
					qsort(SDb1, totalTablesum, sizeof(struct Db),compareCompanyId);
					//Send the company id to Node process 0
					MPI_Send(&totalTablesum,1,MPI_INT,0,0,Node_Comm);
					MPI_Send(&SDb1[0],totalTablesum,MPI_Db,0,0,Node_Comm);
				}
		
				//===============================If Choice 2: Sales by Date=================================================================
		
				else if(crecvBuffer[0].number==2){
					//Sort data depend on the date total number
					int starttimesum=caculateTime(crecvBuffer[0].startyear,crecvBuffer[0].startmonth,crecvBuffer[0].startday);
					int finaltimesum=caculateTime(crecvBuffer[0].finalyear,crecvBuffer[0].finalmonth,crecvBuffer[0].finalday);					

					//Get the number of in date range
					for(i=0;i<totalTablesum;i++){
						int SDb1timesum=0;
						SDb1timesum=caculateTime(SDb1[i].year,SDb1[i].month,SDb1[i].day);
						if(SDb1timesum>=starttimesum&&SDb1timesum<=finaltimesum){
							datacount++;
						}		
					}

					//Put the right number of date in a buffer
					struct Db *DataSDb1=(struct Db*)malloc(datacount*sizeof(struct Db));
					int c=0;
					for(i=0;i<totalTablesum;i++){
						int SDb1timesum=0;
						SDb1timesum=caculateTime(SDb1[i].year,SDb1[i].month,SDb1[i].day);
						if(SDb1timesum>=starttimesum&&SDb1timesum<=finaltimesum){
							DataSDb1[c].sales_id=SDb1[i].sales_id;
							DataSDb1[c].year=SDb1[i].year;
							DataSDb1[c].month=SDb1[i].month;
							DataSDb1[c].day=SDb1[i].day;
							DataSDb1[c].company_id=SDb1[i].company_id;
							memcpy(DataSDb1[c].company_name,SDb1[i].company_name,sizeof(char) * 64);
							DataSDb1[c].sales_total=SDb1[i].sales_total;
							c++;
						}	
					}

					//Sort data depend on the date total number
					qsort(DataSDb1, datacount, sizeof(struct Db),compareTime);
					
					//Send the sort right date buffer to Node process 0
					MPI_Send(&datacount,1,MPI_INT,0,0,Node_Comm);
					MPI_Send(&DataSDb1[0],datacount,MPI_Db,0,0,Node_Comm);

				}
				else if(crecvBuffer[0].number==3){
					test=3;
				}
			}
		}
		else{}

		MPI_Barrier(MPI_COMM_WORLD);
	
	
		//===================================================Final answer========================================================================
	
		struct FinalPrint *FinDb1=(struct FinalPrint*)malloc(max_company_id*sizeof(struct FinalPrint));
	
		int Finnum1=0;
		int Finnum2=0;
		int finpointer1=0;
		int finpointer2=0;
		if(my_rank==0){
			if(csendBuffer[0].number==1){
				//Print choice 1 of the final answer 
				for(i=0;i<eo_p;i++){
					MPI_Recv(&Finnum1,1,MPI_INT,i,0,EO_Comm,&status);
					MPI_Recv(&FinDb1[finpointer1],Finnum1,MPI_FinalPrint,i,0,EO_Comm,&status);
					finpointer1=finpointer1+Finnum1;
				}
				for(i=0;i<max_company_id;i++){
					printf("processes::%i\t%i\t%s\t\t%0.2f\n",my_rank,FinDb1[i].company_id,FinDb1[i].company_name,FinDb1[i].sales_total);
				}
			
			}
			else if(csendBuffer[0].number==2){
				//Print choice 2 of the final answer 
				struct TimePrint *FinDb2=(struct TimePrint*)malloc(try*sizeof(struct TimePrint));
				for(i=0;i<eo_p;i++){
					MPI_Recv(&Finnum2,1,MPI_INT,i,0,EO_Comm,&status);
					MPI_Recv(&FinDb2[finpointer2],Finnum2,MPI_TimePrint,i,0,EO_Comm,&status);
					finpointer2=finpointer2+Finnum2;
				}
				//printf("finpointer2::%i\n",finpointer2);
				for(i=0;i<finpointer2;i++){
					printf("processes::%i\t %i \t %i \t %i \t %f \n",my_rank, FinDb2[i].year, FinDb2[i].month, FinDb2[i].day, FinDb2[i].sales_total);
				}			
			}
			else if(csendBuffer[0].number=3){
				test=3;
			}	
		}

	}
	MPI_Finalize(); 
	
	
	return 0;
}
