/*
 * find.c
 *
 *  Created on: Sep 26, 2015
 *      Author: haoc_wan
 */
#include<stdio.h>
#include <dirent.h>
#include<sys/types.h>
#include <sys/stat.h>
#include "utility.h"




void find(){

	//Find i want to read file;
	char textNameArray[256];
	printf("File Name:");
	scanf("%s", textNameArray);
	FILE *file;
	file = fopen (textNameArray,"r");

	if (file==NULL)
	{

		printf("No file!\n");
		printf("\n");
		return mainMenu();
	}
	//If find my file;
	//Find i want to search word;
	//Reread and to make all information in this file to strings;
	//Finally, print the number of line and the information in this line;
	else{
		char findText[10];
		char myString[100];
		printf("Enter text to find:");
		scanf("%s", &findText);

		int count=0;
		while(fgets(myString,100,file)!=NULL){
			count++;
			if((strstr(myString,findText))!=NULL){
				printf("%i\t%s",count,myString);
			}
		}
		printf("\n");
		return mainMenu();

	}
	fclose(file);





}

