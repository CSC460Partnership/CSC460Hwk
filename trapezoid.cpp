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
#include "trapSlave.h"

using namespace std;

int pipes[9][2];


int main(int argc, char* argv[]){
    //------------------ Error Checking -------------------------
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
    int numTraps = atoi(argv[3]);
    int numSlaves = atoi(argv[4]);
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
    //------------------------------------------------------------------------
    int status,pid;
    float left = atof(argv[1]);
    float right = atof(argv[2]);
    int trapCounter = 0;
    
    float delta = ((right - left) / numTraps);
    float totalArea = 0;
    float message;
    int counter = 1;
    trapSlave dummy;
        
    status = pipe(pipes[0]);
    status = pipe(pipes[8]);
    if(status == -1){
        perror("Pipe did not work");
        exit(1);
    }
    for(int i = 0; i < numTraps; i ++){
        // Make the dummy object
        dummy.left = left + i*delta;
        dummy.right = dummy.left + delta;
        dummy.delta = delta;
        counter ++;
        
        cout << "Iteration: " << i << endl;
        cout << "   left: " << dummy.left << endl;
        cout << "   right: " << dummy.right << endl;
        // Pipey stuff below here...
        pid = fork();
        if(pid == -1){
            perror("Trouble");
            exit(2);
        }
        else if (pid == 0){  // Child code
            cout << "Child created" << endl;
            write(pipes[0][1], &dummy, sizeof(dummy));
            cout << "write successful" << endl;
            execl("child","", to_string(&pipes[0][0]).c_str(), to_string(&pipes[8][1]).c_str(),NULL);
            cout << "execl did not work" << endl;
            return 0;
        }
        else{
            //close(pipes[0][1]);
            read(pipes[8][0], &message, sizeof(float));
            cout << "Received message" << endl;
            totalArea += message;
        }
    }
    cout << "Approximate Area: " << totalArea << endl;
        
    return 0;
}
