# This makefile builds Webserver

# Compiler
CC=g++

# Compiler flags
CFLAGS= -g

# BUILD ALL
all: $(EXECUTABLE)

# Defining constants
WEBSERVER_SOURCES=webserver.cpp \
				  parser.cpp \
				  HttpResponder.cpp

WEBSERVER_OBJECTS=$(subst .cpp,.o,$(WEBSERVER_SOURCES))

EXECUTABLE=webserver



$(EXECUTABLE): $(WEBSERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(WEBSERVER_OBJECTS)

# Express dependencies
parser.o: parser.hpp
webserver.cpp: parser.hpp
	#$(CC) webserver.cpp parser.cpp -o $(EXECUTABLE)

# CLEAN
clean: 
	rm -fr *.o $(EXECUTABLE)


