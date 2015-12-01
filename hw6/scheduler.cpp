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
	3. type:    './scheduler'  for a HELP message
	DISTRIBUTION/SUBTASKS:
			Aaron Hinckley: 50%

            Zach Metcalf: 50%

=============================================================*/

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "info.h"
#include <vector>

using namespace std;

int pipeToChild[2];
int bytes;
int timer = 0;
int procCounter = 0;
const int rHead = 0, wHead = 1;
vector<Info> readyQ;
vector<Info> storage;
vector<Info> output;

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
void displayInfo(vector<Info> temp){
	cout << "------------------------------------------------------" << endl;
	for(int i = 0; i < temp.size(); i ++){
		cout << "Process number: " << temp.at(i).procNumber << endl;
		cout << "	CPU Burst Time: " << temp.at(i).burst << endl;
		cout << "	Arrival Time: " << temp.at(i).arrivalTime << endl;
		cout << "	Wait Time: " << temp.at(i).waitTime << endl;
	}
    cout << "------------------------------------------------------" << endl;

}
void sortQ(vector<Info>& readyQ){
	vector<Info> temp = readyQ;
	int counter = 0;
	int index = 0;
	// Sorted Iteratively
	while(counter != readyQ.size()){
		index = 0;
		for(int i = 0; i < temp.size(); i ++){
			if(temp[i].burst < temp[index].burst){  // SJF Sorting
				index = i;
			}
			else if(temp[i].burst == temp[index].burst){  // FCFS Sorting
				if(temp[i].arrivalTime < temp[index].arrivalTime){
					index = i;
				}
			}
		}
		readyQ[counter] = temp[index];
		temp.erase(temp.begin()+index);
		counter ++;
	}
}
void sortStorage(vector<Info>& storage){
	vector<Info> temp = storage;
	int counter = 0;
	int index = 0;
	// Sorted Iteratively
	while(counter != storage.size()){
		index = 0;
		for(int i = 0; i < temp.size(); i ++){
			if(temp[i].arrivalTime <= temp[index].arrivalTime){
				index = i;
			}
		}
		storage[counter] = temp[index];
		temp.erase(temp.begin()+index);
		counter ++;
	}
}
int popuQ(vector<Info>& storage, vector<Info>& readyQ, int index){
	//displayInfo(storage);
	for(int i = index; i < storage.size(); i ++) {
		if(storage.at(i).arrivalTime <= timer) {
			readyQ.push_back(storage.at(i));

		}
		else {
			break;
		}
		return i;
	}
	//displayInfo(readyQ);
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
		// Scheduler Code Here
		while(procCounter != n){
		//for(int i = 0; i < n; i ++){
			storage.push_back(readPipe());
			procCounter ++;
		//	timer += readyQ.at(i).arrivalTime;
		}
	//	displayInfo(storage);

	// start of Scheduler
		sortStorage(storage); // sort storage based on arrival time
		timer = storage.at(0).arrivalTime; // set initial timer to the earliest
																			 // arrival time
		displayInfo(storage);
		cout << timer << endl;
		readyQ.push_back(storage.at(0)); // add first process to readyQ
		timer += readyQ.at(0).burst;
		output.push_back(readyQ.at(0)); // add to output process done
		readyQ.erase(readyQ.begin());
		cout << "new timer " << timer << endl;

		// look thru storage and add to readyQ
		int index = popuQ(storage, readyQ, 1); // added to readyQ
		// already sorted from storage
		cout << "storage" << endl;
		displayInfo(storage);
		displayInfo(readyQ);
		// works from here up

		while (index < storage.size()) {
			output.push_back(readyQ.at(0));
			timer += readyQ.at(0).burst;
			readyQ.erase(readyQ.begin());
			index = popuQ(storage, readyQ, index);
			cout << "index: " << index << endl;
		}

		cout << "output" << endl;
		displayInfo(output);









		/*
		popuQ(storage,readyQ,0); // add from storage to readyQ if
		sortQ(readyQ);
		displayInfo(readyQ);
		timer += readyQ.at(0).burst;
	
		for(int j = 1; j < storage.size(); ++j) {
			cout << "Iteration: " << j << endl;
			
			popuQ(storage, readyQ, j); // add from storage to readyQ if arrival time is
																 // less than the current time
			displayInfo(readyQ);
			sortQ(readyQ); // sort readyQ by burst time
			readyQ.at(j).waitTime = timer; // add wait time to struct
			timer += readyQ.at(j).burst; // add current process time to the timer
			output.push_back(readyQ.at(j)); // populate output
	
		}
		cout << "output" << endl;
		displayInfo(output);

		/*
		while(storage.size() != 0){
			popuQ(storage,readyQ,readyQ.size());
			sortQ(readyQ);
			readyQ.at(0).waitTime = timer;
			output.push_back(readyQ.at(0));
			//readyQ.erase(readyQ.begin());
			timer += readyQ.at(0).burst;
		}
		*/

		//cout << "timer: " << timer << endl;
		//displayInfo(output);
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
