/*=============================================================
	COURSE:				CSC460, assignment 6
	PROGRAMMERS: 		Aaron Hinckley (ah) and Zach Metcalf (zm)
	MODIFIED BY: 		N/A
	LAST MODIFIED DATE:	11-17-2015
	DESCRIPTION:
	NOTE:					Approximately 1 in 50 times this is executed there
								is a segmentation fault.

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
#include <fstream>
#include <iomanip>
#include <stdio.h>

using namespace std;

int pipeToChild[2];
int bytes;
int timer = 0;
int procCounter = 0;
int storageCounter = 0;
const int rHead = 0, wHead = 1;
vector<Info> readyQ;
vector<Info> storage;
vector<Info> output;

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
void popuQ(vector<Info>& storage, vector<Info>& readyQ){
	int stop;
	for(int i = 0; i < storage.size(); i ++) {
		if(storage.at(i).arrivalTime <= timer) {
			readyQ.push_back(storage.at(i));
			stop = i;
		}
	}
	storage.erase(storage.begin(),storage.begin() + stop+1);
}
void display(vector<Info> output) {
	ofstream myfile;
	myfile.open("record.txt");
	myfile << setw(15) << "Arrival Time";
	myfile << setw(15) << "Process";
	myfile << setw(15) << "CPU Burst";
	myfile << setw(15) << "Waiting Time" << endl;
	for (int i = 0; i < output.size(); ++i)
	{
		myfile << setw(15) << output.at(i).arrivalTime << setw(15) << output.at(i).procNumber << setw(15)
		<< output.at(i).burst << setw(15) << output.at(i).waitTime << endl;
	}
	myfile.close();
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
			storage.push_back(readPipe());
			procCounter ++;
		}
		//--------- Initialize by Adding Processes number 1 to queue -------
		readyQ.push_back(storage.at(0));
		storage.erase(storage.begin());
		timer += readyQ.at(0).burst;  // Advance timer
		output.push_back(readyQ.at(0));  // Send to the CPU
		readyQ.erase(readyQ.begin());  // Delete from Queue
		while(storage.size() != 0){
			popuQ(storage,readyQ);
			while(readyQ.size() != 0){
				output.push_back(readyQ.at(0));
				output.at(output.size()-1).waitTime = timer - readyQ.at(0).arrivalTime;
				readyQ.erase(readyQ.begin());
				timer += output.at(output.size()-1).burst;
			}
		}
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

	display(output);
	storage.resize(0);
	readyQ.resize(0);
	output.resize(0);

	return 0;
}
