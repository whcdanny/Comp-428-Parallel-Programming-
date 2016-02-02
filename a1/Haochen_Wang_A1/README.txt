
----------------------------------------------

This programming is working and work successful in Linux System.

There are 6 files, utility.c, utility.h, find.c, find.h, try.text, news.text. 
1. utility.c: is main file, there are have all the mian functions except find() function.
	(1). int main(): main function start to run all project.
	(2). void mainMenu(): the main menu includes 
		a. File Explorer - connect to the function fileExplorer().
		b. Find - connect to the function find() in the find.c file 
		c. Exit
	(3). void fileExplorer(): the file explorer surface
		a. File List(sorted by name) - connect to the function fileListName().
		b. File List(sorted by size) - connect to the function fileListSize()
		c. Change Directory - connect to changeDirectory().
		d. Main Menu - go back to main menu using mainMenu();
	(4). void fileListName() - sorted the file name by in ascending. 
		a. I used function int nameCompare() to compare two file names.
	(5). void fileListsize() - sorted the file name by their size. 
		a. I used function int sizeCompare() to compare two file sizes.
	(6). void changeDirectory(): to change a directory.

2. utility.h: is a header file for utility.c file, and includes all the funtions of utility.c.

3. find.c: only have the find() function, to find you the file name, and search the text.

4. find.h: is a header file for find.c, and includes the function of find.c.

5. try.text and new.text are text file, to help me to check find() function working.