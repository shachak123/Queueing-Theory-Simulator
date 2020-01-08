CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -pedantic-errors

simulator: simulator.o
	$(CXX) $(CXXFLAGS) -o simulator simulator.o
	
simulator.o: simulator.cpp
	$(CXX) $(CXXFLAGS) -c simulator.cpp