/*=============================================================================
 PROGRAMMER:         Yang Wang
 COURSE:             CSC460: Operating System
 MODIFIED BY:        N/A
 LAST MODIFIED DATE: 10/31/2015 
 DESCRIPTION:        This function is to display the contents of a buffer with
                       a marker underneath to indicate the location of a product
                       that is just put in by the producer or took out by the
                       consumer. 
 NOTE:               This function is used by both producer and consumer threads. 
 FILES:              showBuf.cpp, pc.h
 COMPILER:           GNU g++
 INSTRUCTION FOR COMPILATION:
     type: c++11 -c showBuf.cpp -o showBuf.o
==============================================================================*/
#include "pc.h"

void showBuffer(int status, const int buf[], int size, int markerLoc, char marker)
{const int leftMargin = 14, dataWidth = 5;

 if (status == Producer)                            // beginning
   cout <<setw(leftMargin) <<"\nProducer --> [";      
 else                      
   cout <<setw(leftMargin) <<"\n             [";
 for (int j=0; j<size; j++)	                    // the content of the buffer
   cout <<setw(dataWidth) <<buf[j];

 if (status == Consumer)                            // ending                 
   cout <<" ] <-- Consumer\n";   
 else
   cout <<" ]\n";

 if (status != Init)                                // marker
 {cout <<setw(leftMargin) <<' ';
  for (int j=0; j<size; j++)
   if (j == (markerLoc + size - 1) % size)
     cout <<setw(dataWidth) <<marker;
   else
     cout <<setw(dataWidth) <<' ';
 }
}
