/*=============================================================
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
#include <cstdlib>
#include "info.h"

using namespace std;

int pipeToChild[2];
int bytes;
const int rHead = 0, wHead = 1;

Info recieved, sent;

void writePipe(Info temp) {
	bytes = write(pipeToChild[wHead], &temp, sizeof(temp));
	if (bytes != sizeof(temp)) {
		cout << "Error in write to pipe" << endl;
		exit(EXIT_FAILURE);
	}
}

Info readPipe() {
	Info temp;
	bytes = read(pipeToChild[rHead], &temp, sizeof(temp));
	if (bytes != sizeof(temp)) {
		cout << "Error in read pipe" << endl;
		exit(EXIT_FAILURE);
	}
	return temp;
}

int main(int argc, char* argv[])
{
	// arg error handeling
	if (argc != 2) {
		cout << "Incorrect number of arguments" << endl;
		cout << "Correct Usage: ./scheduler [number of processes]" << endl;
		exit(EXIT_FAILURE);
	}
	const int n = atoi(argv[1]);
	// make sure n is positive
	if (n < 0) {
		cout << "The number must be positive" << endl;
		exit(EXIT_FAILURE);
	}

	if (pipe(pipeToChild) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t returnVal = fork();
	if (returnVal > 0) {
		// parent process

	}
	else if (returnVal < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else {
		// child process
		execl("generator","", &pipeToChild[rHead], &pipeToChild[wHead], argv[1], NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}

	close(pipeToChild[rHead]);
	close(pipeToChild[wHead]);

	return 0;
}
