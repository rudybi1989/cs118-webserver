# This makefile builds Webserver

# Compiler
CC=g++

# Compiler flags
SOURCES=webserver.cpp parser.cpp
EXECUTABLE=webserver



all: 
	$(CC) $(SOURCES) -o $(EXECUTABLE)

clean: 
	rm -fr *.o $(EXECUTABLE)


