/*=============================================================================
 PROGRAMMER:         Yang Wang
 COURSE:             CSC460: Operating System
 MODIFIED BY:        N/A
 LAST MODIFIED DATE: 10/31/2015 
 DESCRIPTION:        This header file is used by ProducerConsumer.cpp and 
			showBuf.cpp 
 NOTE:               N/A
 FILE:               pc.h
==============================================================================*/
#ifndef PC_H_INCLUDED
#define PC_H_INCLUDED

#include <iostream>
using namespace std;
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iomanip>

#define Init     0                  // status used for output controls
#define Producer 1
#define Consumer 2

// This function is for displaying the contents of the shared buffer
void showBuffer(int status, const int buf[], int size, int markerLoc, char marker);

#endif

