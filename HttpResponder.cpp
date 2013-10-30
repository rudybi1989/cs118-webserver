#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include "HttpResponder.h"
#include "parser.hpp"
#include <fstream>

using namespace std;


HttpResponder::HttpResponder(){
	mRequestData.clear();
}

ssize_t HttpResponder::readRequest(int sockFd){

	char tempBuffer[HttpResponder::BYTE_READ_SIZE];
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

	std::cout << std::endl << "Total Bytes Read is: " << totalBytesRead << std::endl;

	mRequestData.clear();
	mRequestData.append(tempBuffer);

	return totalBytesRead;
};

void HttpResponder::logRequestToConsole(){

	std::cout << "The request data size is: " << mRequestData.length() << std::endl;
	std::cout << std::endl;
	std::cout << mRequestData << std::endl;

	return;
}

void HttpResponder::processRequest() {

	ClientRequestMsgDecode msgDecoder((char*) mRequestData.c_str());
};

void HttpResponder::generatePacket(){};

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


void HttpResponder::ReadAndWriteBinary(string FileLocation)
{
  char buffer[1024]; 
  int BlockSize = 1024;
  int BytesRemaining = 0;
  int BytesToRead = 0;
  ifstream is (FileLocation.c_str(), std::ifstream::binary);
  ofstream outfile ("new.html",std::ofstream::binary);

  if (is) 
  {
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    BytesRemaining = length;
    while (BytesRemaining > 0 && is)
    {
      if (BlockSize > BytesToRead)
        BytesToRead = length;
      else
        BytesToRead = BlockSize;
      is.read (buffer, BytesToRead);
      BytesRemaining -= BytesToRead;
      outfile.write (buffer, BytesToRead);
    }

    if (!is)
      cout << "error: only " << is.gcount() << " could be read";
    is.close();
    outfile.close ();
  }
  else
  {
    cout << "File not found.";
  }  
}
