/*=============================================================================
	PROGRAMMER:         Yang Wang Zach Metcalf Aaron Hinckley
	COURSE:             CSC460: Operating System
	MODIFIED BY:        N/A
	LAST MODIFIED DATE: 10/31/2015 
	DESCRIPTION:        A producer thread receives the count of the products n
		through a command-line argument and creates n products
		(simulated with n unique non-negative integers generated
		randomly) for the consumer thread to consume. The two
		threads share a bounded buffer of size bufSize.
	NOTE:               The program is incomplete currently, and synchronization
		between the two threads' operations are needed.
	FILES:              ProducerConsumer.cpp, showBuf.cpp, pc.h, makefile
	COMPILER:           GNU g++
	INSTRUCTION FOR COMPILATION AND EXECUTION:
	1. type: make                                      to COMPILE 
	2. type: ./pc  productCount                        to EXECUTE
==============================================================================*/
#include "pc.h"
#include <semaphore.h>

// global data shared by both producer and consumer threads
int const bufSize = 5;
int       buffer[bufSize];         // space to store the products

pthread_mutex_t lock;
sem_t empty, full;

//*****************************************************************************
void* consumerThread(void* arg);

//*****************************************************************************
int main(int argCount, char* argList[]) 
{
	pthread_t  threadId;
	int        productCount,     // the number of the products to be produced
		       aProduct,         // a product is an integer: 0~999
			   in = 0;           // index for the next generated product

	sem_init(&empty, 0, bufSize); // initialize the empty semaphore to the size of the buffer


	//--------------------------------------
	// 1. Validate the command-line arguments
	//--------------------------------------
	if (argCount != 2)
	{
		cout <<"\n************************************************\n"
			 <<"The correct useage should be: " <<argList[0] <<" productCount"
			 <<"\n************************************************\n\n";
		exit(0);
	}
	productCount = atoi(argList[1]);
	if (productCount < 1)
	{
		cout <<"\nSince the productCount " <<argList[1]
			 <<" < 1, no product will be produced.\n\n";
		exit(0);
	}

	//--------------------------------------
	// 2. Other preparations
	//--------------------------------------
	for (int j=0; j<bufSize; j++)     // A "-1" placed in the buffer is used as only
		buffer[j] = -1;               // a visual clue for an "empty" slot of the
									  // buffer in the output, and it should not be
									  // uesd for any condition test in the program
	showBuffer(Init, buffer, bufSize, 0, ' ');

	srand(time(NULL));              // for simulating time laps

	//--------------------------------------
	// 3. Create the consumer thread
	//--------------------------------------
	if (pthread_create(& threadId, NULL, consumerThread,  & productCount) != 0)
	{
		cout <<"\nError: cannot create the consumer thread.\n";
		exit(0);
	}

	//--------------------------------------
	// 4. Producer produces productCount items
	//--------------------------------------
	for (int i=0; i<productCount; i++)
	{
		sleep(rand()%4);               // simulate the time for producing a product
		aProduct = rand()%1000;        // products are integers: 0~999
		sem_wait(&empty);
		pthread_mutex_lock(&lock);
		buffer[in] = aProduct;         // place the product into the buffer  
		in = (in+1) % bufSize;                    
		showBuffer(Producer, buffer, bufSize, in, '#');   // '#' marks the last 
		// in-product position
		pthread_mutex_unlock(&lock);
		sem_post(&full);
	}

	//--------------------------------------
	// 5. Wait for the consumer thread to finish
	//--------------------------------------
	if (pthread_join(threadId, NULL) != 0)
		cout <<"Failed to wait for the consumer thread.\n";

	//--------------------------------------
	// 6. Wrap-up
	//--------------------------------------
	cout <<"\nDone.\n";
	return 0;
}

//*****************************************************************************
void* consumerThread(void* arg)
{
	int productCount = * (int*) arg, // retrieve the count of the products
	out = 0;                     // index for the next to-be-consumed product

	for (int i=0; i<productCount; i++)
	{
		sem_wait(&full);
		pthread_mutex_lock(&lock);
		buffer[out] = -1;              // remove a product from the buffer
		out = (out + 1) % bufSize;
		showBuffer(Consumer, buffer, bufSize, out, '*');   // '*' marks the last
		// out-product position
		sleep(rand()%4);               // simulate the time for consuming the product
		pthread_mutex_unlock(&lock);
		sem_post(&empty);
	}

	pthread_exit(NULL);
}

