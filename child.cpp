

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "trapSlave.h"

using namespace std;

int main(int argc, char* argv[]){
    float area;
    int* source = (int*) argv[1];
    int* destination = (int*) argv[2];
    trapSlave task;
    int* t1;
    char buf[sizeof(task)];    
    
    read(*source, buf,sizeof(task));
    t1 = (trapSlave*) buf;
    
    task = *t1;
    cout << task.left << endl;
    area = task.area();
    cout << "The child calculated area: " << area << endl;
    write(*destination, &area, sizeof(float));
    return 0;
    
    
}