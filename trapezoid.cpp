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
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
#include "trapSlave.h"

using namespace std;

int pipes[9][2];
int slaveJobCount[8];
int numTraps, numSlaves, trapIndex, bytes;
int iteration = 0;
const int rHead = 0, wHead = 1;
float a, b, totalArea, delta;
trap recvTrap;
trap sendTrap;

trap makeTrap(int s, float delta){
    trap temp;
    temp.a = a + (trapIndex * delta);
    temp.b = temp.a + delta;
    temp.delta = delta;
    temp.slaveNum = s;
    return temp;
}
void writeTrap(trap temp){
    bytes = write(pipes[temp.slaveNum][wHead], &temp, sizeof(temp));
    if(bytes != sizeof(temp)){
        cout << "Error in master write to pipe" << endl;
        exit(0);
    }
    //cout << "Process number " << s << " executing" << endl;
}
trap readTrap(){
    trap temp;
    bytes = read(pipes[8][rHead], &temp, sizeof(temp));
    if(bytes != sizeof(temp)){
        cout << "Parent read error" << endl;
        exit(0);
    }
    return temp;
}
int main(int argc, char* argv[]){
    //----------------- - Error Checking -------------------------
    if(argc < 5){
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
    numTraps = atoi(argv[3]);
    numSlaves = atoi(argv[4]);
    a = atof(argv[1]);
    b = atof(argv[2]);
    trap trapBox[numTraps];
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
    trapIndex = 0;
    //------------ Create Pipes Needed ----------------------
    for(int i = 0; i < min(numTraps,numSlaves); i ++){
        if(pipe(pipes[i]) == -1){
            cout << "Pipe number " << i << " failed" << endl;
            exit(0);
        }
    }
    pipe(pipes[8]);
    // ----------- Make trapezoids -------------
    for(int t = 0; t < numTraps; t ++){  
        trapBox[t] = makeTrap(0,delta);
        trapIndex ++;
    }
    // Create Slaves and start processing
    for(int i = 0; i < 8; i ++){
        slaveJobCount[i] = 0;
    }
    
    trapIndex = 0;
    for(int s = 0; s < min(numSlaves, numTraps); s ++){
        pid_t returnVal = fork();
        switch(returnVal){
        case -1:
            cout << "Fork failed" << endl;
            exit(0);
        case 0:  // child code
            execl("child","",&pipes[s][rHead], &pipes[8][wHead],NULL);
            cout << "execl failed" << endl;
            exit(0);
        default : // parent code-- initialize the sending and recv
            trapBox[s].slaveNum = s;
            trapBox[s].n = s;
            writeTrap(trapBox[trapIndex]);
            //cout << "Init with trapezoid " << trapIndex << endl;
            trapIndex ++;
            //cout << "Sent endpoints: [" << sendTrap.a << "," << sendTrap.b << "]" << endl;
        }
    }
    int trapsUsed = 0;
    //cout << "Starting trapIndex: " << trapIndex << endl;
    for(;;){// master is listening, feed trapezoids :)
        trap recvTrap = readTrap();
        trapsUsed ++;
        totalArea += recvTrap.area;
        slaveJobCount[recvTrap.slaveNum] ++;
        //cout << "Process number " << recvTrap.slaveNum << " finished" << endl;
        //cout << "   trapIndex: " << trapIndex << endl;
        //cout << "   trapsUsed: " << trapsUsed << endl;
        //cout << "   trap passed: " << recvTrap.n << endl;
        //cout << "   Interval: " << "[" << recvTrap.a << "," << recvTrap.b << "]" << endl;
        //cout << "   Delta: " << delta << endl;
        //cout << "   Added area: " << recvTrap.area << endl;
        //cout << "   Total area: " << totalArea << endl;
        //cout << "   With job count: " << slaveJobCount[recvTrap.slaveNum] << endl;
        if(trapIndex < numTraps){  // Still more trapezoids to send
            trapBox[trapIndex].slaveNum = recvTrap.slaveNum;
            trapBox[trapIndex].n = trapIndex;
            writeTrap(trapBox[trapIndex]);
            trapIndex ++;
        }
        else{
            while(trapsUsed < numTraps){  // a child needs to be read still
                trap recvTrap = readTrap();
                trapsUsed ++;
                totalArea += recvTrap.area;
                slaveJobCount[recvTrap.slaveNum] ++;
                //cout << "Process number " << recvTrap.slaveNum << " finished" << endl;
                //cout << "   trapIndex: " << trapIndex << endl;
                //cout << "   trapsUsed: " << trapsUsed << endl;
                //cout << "   trap passed: " << recvTrap.n << endl;
                //cout << "   Interval: " << "[" << recvTrap.a << "," << recvTrap.b << "]" << endl;
                //cout << "   Delta: " << delta << endl;
                //cout << "   Added area: " << recvTrap.area << endl;
                //cout << "   Total area: " << totalArea << endl;
                //cout << "   With job count: " << slaveJobCount[recvTrap.slaveNum] << endl;
            }
            break;
        }
        
    }
    int max = 0;
    int maxValue = 0;
    for(int i = 0; i < numSlaves; i ++){
        cout << "Slave number " << i << " trapezoid(s): " << slaveJobCount[i] << endl;
        if(slaveJobCount[i] >= maxValue){
            max = i;
            maxValue = slaveJobCount[i];
        }
    }
    cout << "============================================================================" << endl;
    cout << "Congratulations to process number " << max << " for doing the most work!!!" << endl;
    cout << "==============================================================================" << endl;
    // Terminate all children processes and close all pipes
    for(int s = 0; s < min(numSlaves,numTraps); s ++){
        trap sendTrap = makeTrap(s,-1);
        writeTrap(sendTrap);
        close(pipes[s][0]);
        close(pipes[s][1]);
    }
    close(pipes[8][0]);
    close(pipes[8][1]);
    cout << "Approximate Area: " << totalArea << endl;
    return 0;
    }
