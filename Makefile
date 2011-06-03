# Makefile for Randomized Algorithms
CXXFLAGS = -Wall -g
CXX=g++

analyse: analyse.o BloomFilter.o SkipList.o RBST.o
	$(CXX) analyse.o BloomFilter.o SkipList.o RBST.o -o analyse -lm -lstdc++

analyse.o: analyse.cpp

BloomFilter.o: BloomFilter.cpp BloomFilter.hpp

SkipList.o: SLTest.cpp SkipList.cpp SkipList.hpp

RBST.o: RBST.cpp RBST.hpp

clean:
	rm -f *.o
