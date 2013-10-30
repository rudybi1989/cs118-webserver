# This makefile builds Webserver

# Compiler
CC=g++

# Compiler flags
CFLAGS= -g

# BUILD ALL
all: $(EXECUTABLE)

# Defining constants
WEBSERVER_SOURCES=webserver.cpp parser.cpp HttpResponder.cpp

WEBSERVER_OBJECTS=$(subst .cpp,.o,$(WEBSERVER_SOURCES))

EXECUTABLE=webserver


$(EXECUTABLE): $(WEBSERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(WEBSERVER_OBJECTS)

# Express dependencies
parser.o: parser.cpp parser.hpp
HttpResponder.o: HttpResponder.cpp HttpResponder.h parser.cpp parser.hpp
webserver.o: webserver.cpp parser.cpp parser.hpp HttpResponder.cpp HttpResponder.h

# CLEAN
clean: 
	rm -fr *.o $(EXECUTABLE)


