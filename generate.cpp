// Authors: Aaron Hinckley and Zach Metcalf
// Grok users ah and zm

#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){
        
    if(argc != 2){
        cout << "Process number not determined!" << endl;
        cout << "This program requires exactly ONE command line integer argument!!!" << endl;
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
                cout << "Child process could not be created! Aborting program!" << endl;
                return 0;
            }
            else if(returnValue == 0){  // Child process
                cout << "Child number " << i << " with pid " << getpid() << " and parent pid " << getppid() << endl;
                if(i % 2 == 0){ // Even child, create grandchild 
                    returnValue = fork();
                    //cout << returnValue << endl;
                    if(returnValue == -1){
                        cout << "       Grandchild process could not be created! Aborting program!" << endl;
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
    




