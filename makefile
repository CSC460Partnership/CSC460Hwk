# makefile for trapezoid.cpp

trapezoid: trapezoid.cpp child.o trapSlave.o
	g++ trapezoid.cpp child.o trapSlave.o -o trapezoid

child: child.cpp
	g++ -c child.cpp -o child.o

trapSlave: trapSlave.cpp
	g++ -c trapSlave.cpp -o trapSlave.o
