#include<stdio.h>
#include <dirent.h>
#include<sys/types.h>
#include <sys/stat.h>
#include "utility.h"
#include "find.h"


int main(){
	mainMenu();

	return 0 ;

}

void mainMenu(){

	int choice1;

	printf("+++++++++++++++++++++++++++++++\n");
    printf("Sys Admin Utility\n");
    printf("\n");
    printf("1.File Explorer\n");
    printf("\n");
    printf("2.Find\n");
    printf("\n");
    printf("4.Exit\n");
    printf("\n");
	printf("+++++++++++++++++++++++++++++++\n");
    printf("\n");
    printf("Selection:");

    scanf("%d",&choice1);

    if(choice1==1){
    	//GO File Explorer;
        system("clear");
        fileExplorer();
    }
    else if(choice1==2){
        find();
        //Go find;
        system("clear");

    }
    else if(choice1==4){
    	//Go close;
        system("clear");
        system("PAUSE");
    }
    else
        system("PAUSE");
}

void fileExplorer(){
    int choice2;

    printf("+++++++++++++++++++++++++++++++\n");
    printf("\n");
    printf("File Explorer\n");
    printf("\n");
    printf("1.File List(sorted by name)\n");
    printf("\n");
    printf("2.File List(sorted by size)\n");
    printf("\n");
    printf("3.Change Directory\n");
    printf("\n");
    printf("4.Main Menu\n");
    printf("\n");
    printf("+++++++++++++++++++++++++++++++\n");
    printf("\n");
    printf("Selection:");

    scanf("%d",&choice2);

    if(choice2==1){
    	//Go file list by name;
        system("clear");
        fileListByName();
    }
    else if (choice2==2){
    	//Go file list by size;
        system("clear");
        fileListBySize();
    }
    else if (choice2==3){
    	//Go changeDirectory;
        system("clear");
        changeDirectory();
    }
    else if(choice2==4){
    	//Go back to main menu;
        system("clear");
        mainMenu();
    }


}

int nameCompare(const void *a,const void *b){
	struct dirent *p1=(struct dirent *)a;
	struct dirent *p2=(struct dirent *)b;
	//Compare to name;
	return strcmp(p1->d_name,p2->d_name);
}

void fileListByName(){
	//Create a directory;
	//Get how many files in your directory;
	DIR *d1;
	struct dirent *dir;
	int count=0;
	d1=opendir(".");
	while((dir=readdir(d1))!=NULL){
		count++;
	}
	closedir(d1);

	//Put all files in a array;
	DIR *d2;
	struct dirent *dirArray;
	dirArray = malloc(count *sizeof(struct dirent));
	d2=opendir(".");

	int i=0;
	while((dir=readdir(d2))!=NULL){
		dirArray[i]=*dir;
		i++;
	}
	closedir(d2);

	//Compare files name;
	qsort(dirArray,count,sizeof(struct dirent),nameCompare);

	printf("%s\t%s\t%s\n","FIle_Type","Size(Kb)","FIle_Name");

	//Go to check file type one by one in directory array;
	int j;
	for(j=0;j<count;j++){
		struct stat s;
		stat(dirArray[j].d_name,&s);
		switch (s.st_mode & S_IFMT) {
			case S_IFREG:  printf("Regular");            	  break;
			case S_IFDIR:  printf("Directory");               break;
			case S_IFLNK:  printf("Symlink");                 break;
			case S_IFBLK:  printf("Block Device");            break;
		    case S_IFCHR:  printf("Character Device");        break;
		    case S_IFIFO:  printf("FIFO/pipe");               break;
		    case S_IFSOCK: printf("Socket");                  break;
		    default:       printf("unknown?");                break;
		}

		printf("t%s\t%i\t%s\t%s\n","",s.st_size,"",dirArray[j].d_name);
	}
	fileExplorer();
}

int sizeCompare(const void *a,const void *b){
	struct dirent *p1=(struct dirent *)a;
	struct dirent *p2=(struct dirent *)b;

	struct stat s1;
	struct stat s2;

	stat(p1->d_name,&s1);
	stat(p2->d_name,&s2);
	if(s1.st_size>s2.st_size){
		return 1;
	}
	else if(s1.st_size<s2.st_size){
		return -1;
	}
	else
		return 0;
}


void fileListBySize(){
	//Create a directory;
	//Get how many files in your directory;
	DIR *d1;
	struct dirent *dir;
	int count=0;
	d1=opendir(".");
	while((dir=readdir(d1))!=NULL){
		count++;
	}
	closedir(d1);
	//Put all files in a array;
	DIR *d2;
	struct dirent *dirArray;
	dirArray = malloc(count *sizeof(struct dirent));
	d2=opendir(".");

	int i=0;
	while((dir=readdir(d2))!=NULL){
		dirArray[i]=*dir;
		i++;
	}
	closedir(d2);

	//Compare files size;
	qsort(dirArray,count,sizeof(struct dirent),sizeCompare);

	printf("%s\t%s\t%s\n","FIle_Type","Size(Kb)","FIle_Name");


	//Go to check file type one by one in directory array;
	int j;
	for(j=0;j<count;j++){
		struct stat s;
		stat(dirArray[j].d_name,&s);
		switch (s.st_mode & S_IFMT) {
			case S_IFREG:  printf("Regular");            	  break;
			case S_IFDIR:  printf("Directory");               break;
			case S_IFLNK:  printf("Symlink");                 break;
			case S_IFBLK:  printf("Block Device");            break;
			case S_IFCHR:  printf("Character Device");        break;
			case S_IFIFO:  printf("FIFO/pipe");               break;
			case S_IFSOCK: printf("Socket");                  break;
			default:       printf("unknown?");                break;
		}

		printf("t%s\t%i\t%s\t%s\n","",s.st_size,"",dirArray[j].d_name);
	}
	fileExplorer();
}


void changeDirectory(){
	//Find a directory name;
	char directoryName[256];
	printf("\n");
	printf("Directory name:");
	scanf("%s", directoryName);

	//Check the name whether in this directory;
	if(chdir(directoryName)==0){
		fileExplorer();
	}
	else{
		printf("Cannot find your directory name!\n");
		printf("Please rewrite the directory name.\n");
		changeDirectory();
	}

}










