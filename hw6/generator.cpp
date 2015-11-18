#include <iostream>

using namespace std;
struct Counter {
	int timeLapse;
	int burstTime
};
int main(int argc, char* argv[])
{
	Counter time;
	time.timeLapse = rand()%49;
	time.burstTime = rand()%1000;
	
	
	return 0;
}
