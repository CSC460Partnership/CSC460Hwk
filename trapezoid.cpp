/*=============================================================
	COURSE:	CSC460, assignment 4
	PROGRAMMERS: 	Aaron Hinckley (ah) and Zach Metcalf (zm)
	MODIFIED BY: 	na
	LAST MODIFIED DATE:	9-20-2015
	DESCRIPTION:	This program uses unix pipes to calculate the integral of a function using the
                    trapezoid rule. It uses a master process to spawn at most 8 slave processes
                    each calculating a subinterval trapezoid. The slave processes may calculate more
                    than one interval depending on the user input for number of intervals in the
                    trapezoid rule. 
	NOTE:	na
	FILES:	trapezoid.cpp
	IDE/COMPILER/INTERPRETER:	GNU g++
	INSTRUCTION FOR COMPILATION AND EXECUTION:
	1. type:	g++ --std=c++11 trapezoid.cpp -o trapezoid
	2. type:	./trapezoid left right n m  to EXECUTE with the interval [left,right] and
                                            and integer n intervals and integer m subprocesses
	OR
	3. type:    ./trapezoid  for a HELP message
	DISTRIBUTION/SUBTASKS:  Aaron Hinckley: 50%
                                Initial problem solving strategy
                                Code structure
                            Zach Metcalf: 50%
                                Output formatting
                                Debugging
=============================================================*/

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "trapSlave.h"

using namespace std;

int pipes[9][2];
int slaveJobCount[7];
int numTraps, numSlaves, trapCounter;
float a, b, totalArea, smallArea, delta;

trap makeTrap(float a, float b, float delta){
    trap temp;
    temp.a = a + trapCounter * delta;
    temp.b = temp.a + delta;
    temp.delta = delta;
    return temp;
}


int main(int argc, char* argv[]){
    const int rHead = 0, wHead = 1;
    int bytes, slaveCounter;
    numTraps = atoi(argv[3]);
    numSlaves = atoi(argv[4]);
    a = atof(argv[1]);
    b = atof(argv[2]);
   
    //----------------- - Error Checking -------------------------
    if(argc == 1){
        cout << "=============================================================================" << endl;
        cout << "Correct Usage:  ./trapezoid left right n m " << endl;
        cout << "With interval [left,right], integer n intervals and integer m subprocesses" << endl;
        cout << "Note n > 0 and 0 < m <= 8" << endl;
        cout << "=============================================================================" << endl;
        return 0;
    }
    if(argc != 5){
        cout << "Invalid command line arguments!!!" << endl;
        cout << "Correct Usage:  ./trapezoid left right n m " << endl;
        cout << "Note n > 0 and 0 < m <= 8" << endl;
        return 0;
    }
    if(numTraps <= 0){
        cout << "Invalid trapezoid number n!!! " << endl;
        cout << "n must be greater than zero!!!" << endl;
        return 0;
    }
    if(numSlaves <= 0 || numSlaves > 8){
        cout << "Invalid subprocess number m!!!" << endl;
        cout << "m must be in range (0,8]" << endl;
        return 0;
    }
    if(a > b){
        cout << "Invalid range integers!!!" << endl;
        cout << "In [left,right], left must be greater than right!!!" << endl;
        return 0;
    }
      
    delta = ((b - a) / numTraps);
    totalArea = 0;
    trapCounter = 0;
    slaveCounter = 0;
    trap dummy;
   
    
    //------------ Create Pipes Needed ----------------------
    for(int i = 0; i < min(numTraps,numSlaves)+1; i ++){
        if(pipe(pipes[i]) == -1){
            cout << "Pipe number " << i << " failed" << endl;
            exit(0);
        }
    }
    pipe(pipes[8]);
    // Create Slaves
    //or(int s = 0; s < min(numSlaves, numTraps); s ++){
    while(numTraps != trapCounter){
        pid_t returnVal = fork();
        switch(returnVal){
        case -1:
            cout << "Fork failed" << endl;
            exit(0);
        case 0:  // child code
            cout << "Fork created" << endl;
            execl("child","",&pipes[slaveCounter][rHead], &pipes[8][wHead],NULL);
            cout << "execl failed" << endl;
            exit(0);
        default : // parent code
            dummy = makeTrap(a,b,delta);
            bytes = write(pipes[slaveCounter][wHead], &dummy, sizeof(dummy));
            if(bytes != sizeof(dummy)){
                cout << "Error in master write to pipe" << endl;
                exit(0);
            }
            bytes = read(pipes[8][rHead], &smallArea, sizeof(smallArea));
            if(bytes != sizeof(smallArea)){
                cout << "Parent read error" << endl;
                exit(0);
            }
            cout << "Parent read: " << smallArea << endl;
            totalArea += smallArea;
            slaveJobCount[slaveCounter] += 1;
            trapCounter ++;
            slaveCounter ++;
        }
    }
    cout << "Approximate Area: " << totalArea << endl;
    return 0;
    }
