CXX = g++
CXXFLAGS = -g -std=c++11 -Wall

SRCS = $(wildcard *.hpp)
OBJECTS = $(SRCS: .hpp=.o)

all: server

server: $(OBJECTS) main.cpp packet.hpp packet.cpp
		$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf *.dSYM
		$(RM) *.o *.gc* test/*.o core main

