#include <unistd.h>
#include <iostream>
#include <string>
#include "HttpResponder.h"



HttpResponder::HttpResponder(){
	mRequestData.clear();
}

ssize_t HttpResponder::readRequest(int sockFd){

	char tempBuffer[1024];
	memset(&tempBuffer, '0', sizeof(tempBuffer));
	ssize_t bytesRead = 0;
	ssize_t totalBytesRead = 0;
	
	
	bytesRead = read(sockFd, tempBuffer, HttpResponder::BYTE_READ_SIZE); //read 1024 bytes at a time.
		
	if (bytesRead < 0)
	{
		return -1; //read error
	}

	tempBuffer[bytesRead] = '\0';

	totalBytesRead += bytesRead;

	mRequestData.clear();
	mRequestData.append(tempBuffer);

	return totalBytesRead;
};

void HttpResponder::logRequestToConsole(){

	std::cout << mRequestData << std::endl;

	return;
}

int HttpResponder::writeOnTCP(int sockFd)
{
	std::string output = "Hello World!";
    int bytesWritten = 0;

    bytesWritten = write(sockFd, output.c_str(), output.length());

    if(bytesWritten < 0)
    {
    	return -1;
    }

    return bytesWritten;
}