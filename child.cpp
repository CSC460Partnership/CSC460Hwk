

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "trapSlave.h"

using namespace std;

float arbFunction(float x){
    return (x*x) + (2*x) + 4;
}

float area(float a, float b, float delta){
    return (((arbFunction(a) + arbFunction(b))/2) * delta);
}   

int main(int argc, char* argv[]){
    trap temp;
    int bytes;
    float tempArea;
    float slaveInfo[2];
     
    for(;;){
        bytes = read(*argv[1], &temp, sizeof(temp));
        if(bytes != sizeof(temp)){
            cout << sizeof(temp) << endl;
            cout << bytes << endl;
            cout << "Child read error" << endl;
            close(*argv[1]);
            return 0;
        }
        if(temp.delta == -1){  // Terminate Signal, close pipes
            close(*argv[1]);
            close(*argv[2]);
            break;
        }
        tempArea = area(temp.a,temp.b, temp.delta);
        temp.area = tempArea;
        bytes = write(*argv[2], &temp, sizeof(temp));
        if(bytes != sizeof(temp)){
            cout << "Child write error" << endl;
            close(*argv[2]);
            return 0;
        }
    }
    return 0;
}
    
    
