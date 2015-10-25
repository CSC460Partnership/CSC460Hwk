# makefile for trapezoid.cpp

trapezoid: trapezoid.cpp trapSlave.h child
	g++ --std=c++11 trapezoid.cpp -o trapezoid

child: child.cpp trapSlave.h
	g++ --std=c++11 child.cpp -o child
