#include <iostream>/*=============================================================
	COURSE:				CSC460, assignment 6
	PROGRAMMERS: 		Aaron Hinckley (ah) and Zach Metcalf (zm)
	MODIFIED BY: 		N/A
	LAST MODIFIED DATE:	11-17-2015
	DESCRIPTION:		
	NOTE:					N/A
	FILES:						generate.cpp
	IDE/COMPILER/INTERPRETER:	GNU g++
	INSTRUCTION FOR COMPILATION AND EXECUTION:
	1. type:	'make' while inside the correct directory
	2. type:	'./scheduler [n]'  to EXECUTE with integer n child processes
	OR
	3. type:    './scheudler'  for a HELP message
	DISTRIBUTION/SUBTASKS:  
			Aaron Hinckley: 50%
				
            Zach Metcalf: 50%
                            
=============================================================*/

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[])
{
	int pipe;
	const int rHead = 0, wHead = 1;
	
	// arg error handeling
	if (argc != 2)
	{
		cout << "Incorrect number of arguments" << endl;
		cout << "./scheduler [number of processes]" << endl;
		exit(EXIT_FAILURE);
	}
	if (argv[0] < 0 || argv[0] > 49)
	{
		cout << "The integer you entered needs to be in the range" << 
				"0 < n <= 49" << endl;
		exit(EXIT_FAILURE);
	}
	
	// processes
	int pipeToChld[2];
	
	if (pipe(pipeToChild) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	pid_t returnVal = fork();
	if (returnVal == 0)
	{
		// parent process
		
	}
	else if (returnVal < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		// child process
		execl("generator", '', , NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}



	return 0;
}
