/*=============================================================
	COURSE:	CSC460, assignment 3
	PROGRAMMERS: 	Aaron Hinckley (ah) and Zach Metcalf (zm)
	MODIFIED BY: 	na
	LAST MODIFIED DATE:	9-8-2015
	DESCRIPTION:	This program generates a user defined number of child processes, where every
                    even numbered process generates a grandchild process. The parent
                    displays its pid then produces n number of duplicated child processes.
                    Each child process displays its number, pid, and parent pid. Each grandchild
                    process displays its pid and parent pid. The user integer n must be in range
                    of [1,20]. 
	NOTE:	na
	FILES:	generate.cpp
	IDE/COMPILER/INTERPRETER:	GNU g++
	INSTRUCTION FOR COMPILATION AND EXECUTION:
	1. type:	g++ --std=c++11 generate.cpp -o generate
	2. type:	./generate [n]  to EXECUTE with integer n child processes
	OR
	3. type:    ./generate  for a HELP message
	DISTRIBUTION/SUBTASKS:  Aaron Hinckley: 50%
                                Initial problem solving strategy
                                Code structure
                            Zach Metcalf: 50%
                                Output formatting
                                Debugging
=============================================================*/

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
    
    if(argc == 1){  // Help message
        cout << "=========================================" << endl;
        cout << "Correct Usage:  ./generate [n]" << endl;
        cout << "The integer n needs to be in range [1,20]" << endl;
        cout << "=========================================" << endl;
        return 0;
    }
    if(argc > 2){
        cout << "Process number not determined!" << endl;
        cout << "This program requires exactly ONE command line integer argument in range of [1,20]!!!" << endl;
        return 0;
    }
   
    const int N = atoi(argv[1]);
   
    if(N > 20 || N < 1){
        cout << "Invalid process number!" << endl;
        cout << "Need an integer in the range of 1 to 20 inclusive!!!" << endl;
    }   
    else{
        pid_t returnValue;
        cout << "Parent process pid " << getpid() << endl;
        for(int i = 1; i <= N; i ++){
            returnValue = fork();
            if (returnValue == -1){
                cout << "   Child process could not be created! Aborting program!" << endl;
                return 0;
            }
            else if(returnValue == 0){  // Child process
                cout << "   Child number " << i << " with pid " << getpid() << " and parent pid " << getppid() << endl;
                if(i % 2 == 0){ // Even child, create grandchild 
                    returnValue = fork();
                    //cout << returnValue << endl;
                    if(returnValue == -1){
                        cout << "           Grandchild process could not be created! Aborting program!" << endl;
                        return 0;
                    }
                    else if(returnValue == 0){  // Grandchild process
                        cout << "       Grandchild pid " << getpid() << " with parent pid " << getppid() << endl;
                    }
                    return 0;
                }
                return 0;
            }
        }
    }
    return 0;
}
    




