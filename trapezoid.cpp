/*=============================================================
	COURSE:	CSC460, assignment 4
	PROGRAMMERS: 	Aaron Hinckley (ah) and Zach Metcalf (zm)
	MODIFIED BY: 	na
	LAST MODIFIED DATE:	9-20-2015
	DESCRIPTION:	This program uses unix pipes to calculate the integral of a function using the
                    trapezoid rule. It uses a master process to spawn at most 8 slave processes
                    each calculating subinterval trapezoid. The slave processes may calculate more
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

using namespace std;

class trapSlave{
    public:
    
        float left, right;  // endpoints of individual trapezoid
        float slaveNum;
        float delta;
        float arbFunction(float x){
            return (x*x) + (2*x) + 4;
        }
        float area(){
            cout << "       f(left): " << arbFunction(left) << endl;
            cout << "       f(right): " << arbFunction(right) << endl;
            return (((arbFunction(left) + arbFunction(right))/2) * delta);
        }
};

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
    float left = atof(argv[1]);
    float right = atof(argv[2]);
    int trapCounter = 0;
    
    float delta = ((right - left) / numTraps);
    float totalArea = 0;
    int counter = 1;
    trapSlave dummy; 
    cout << "delta: " << delta << endl;
    for(int i = 0; i < numTraps; i ++){
        // Make the dummy object
        dummy.left = left + i*delta;
        dummy.right = dummy.left + delta;
        dummy.slaveNum = counter;
        dummy.delta = delta;
        counter ++;
        cout << "Iteration: " << i << endl;
        cout << "   left: " << dummy.left << endl;
        cout << "   right: " << dummy.right << endl;
        // Pipey stuff below here...
        
        cout << "   area: " << dummy.area() << endl;
        totalArea += dummy.area();
    }
    cout << "Approximate Area: " << totalArea << endl;
        
    return 0;
}
