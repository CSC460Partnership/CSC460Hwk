# makefile for trapezoid.cpp

trapezoid: trapezoid.cpp child.o
	g++ trapezoid.cpp child.o -o trapezoid

child: child.cpp
	g++ -c child.cpp -o child.o
