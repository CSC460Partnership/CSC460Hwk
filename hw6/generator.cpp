#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "info.h"
#include <time.h>
using namespace std;


int main(int argc, char* argv[])
{
	srand(time(0));
	int timeLapse, burst;
	int prevTime = 0;
	int n = atoi(argv[3]);
	int bytes;
	
	if (argc != 4)
	{
		perror("incorrect args");
		exit(EXIT_FAILURE);
	}
	
	Info temp;
	for (int i = 0; i < n; ++i)
	{
		burst = rand() % 1000 + 1;
		timeLapse = rand() % 50;
		
		temp.arrivalTime = timeLapse + prevTime;
		temp.burst = burst;
		
		prevTime = temp.arrivalTime;
		
		//temp into pipe;
		
		bytes = write(*argv[2], &temp, sizeof(temp));
		if(bytes != sizeof(temp)) {
			perror("write");
		}
		
	}
	
	close(*argv[2]);


	return 0;
}
