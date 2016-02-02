/*
 ============================================================================
 Name        : zipf.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


/*************************************************************************
 * This is a simple program to generate a skewed values in the range
 * 0.0 to 1000.0. It is based on a Zipfian probability distribution and will
 * skew the values towards the high end of the range. The amount of skew
 * will vary each time the program is run.
 *
 * To use the skewed value generator, you must first call the init_skew()
 * method. You may then call get_amount() as many times as yu like. The
 * basic logic is illustrated in the main() method.
 *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


void init_skew();
float get_amount();
void get_zipf(float, int);

struct probvals
{
	float prob;
	float cum_prob;
};

struct probvals *zdist;


int main(int argc, char** argv){

	init_skew();
	int i;

	for(i = 0; i < 20; i++){
		printf("Value: %f\n", get_amount());
	}

	return 0;
}



float get_amount(){
	int index =   rand() % 99000;
	return zdist[index].cum_prob * 1000;
}

void init_skew(){

	time_t t;
	int N = 99000;

	srand((unsigned) time(&t));
	float theta =   rand() % 1000 / 1000.0;

	zdist = (struct probvals *)malloc(N*sizeof(struct probvals));
	get_zipf(theta, N);
}


void get_zipf(float theta, int N){

	float sum=0.0;
	float c=0.0;
	float expo;
	float sumc =0.0;
	int i;

	expo = 1 - theta;

	for (i = 1; i <= N; i++) {
		sum += 1.0 /(float) pow((double) i, (double) (expo));
	}
	c = 1.0 / sum;

	for (i = 0; i < N; i++) {
		zdist[i].prob = c /
				(float) pow((double) (i + 1), (double) (expo));
		sumc +=  zdist[i].prob;
		zdist[i].cum_prob = sumc;
	}
}


