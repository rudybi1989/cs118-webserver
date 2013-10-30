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
	
	bytesRead = read(sockFd, tempBuffer, HttpResponder::BYTE_READ_SIZE); //read 2048 bytes at a time.
		
	if (bytesRead < 0)
	{
		return -1; //read error
	}

	tempBuffer[bytesRead] = '\0'; //add the null terminator

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

	mOutputHeader.clear();
	mOutputData.clear();

	mOutputData.append("<html><b style=\"color:red\">Hello World!</b><html>");

	ClientRequestMsgDecode msgDecoder((char*) mRequestData.c_str());

	mOutputHeader.append("HTTP/1.1 200 OK");
	mOutputHeader.append("\r\n");
	mOutputHeader.append("Content-Type: text/html");
	mOutputHeader.append("\r\n");
	mOutputHeader.append("Content-Length: ");
	mOutputHeader.append(std::to_string((int) mOutputData.size()));
	mOutputHeader.append("\r\n");
	mOutputHeader.append("Connection: close");
	mOutputHeader.append("\r\n");
	mOutputHeader.append("\r\n");

};

void HttpResponder::generatePacket(){};

int HttpResponder::writeOnTCP(int sockFd)
{
	int totalBytesWritten = 0;
    int bytesWritten = 0;

    bytesWritten = write(sockFd, mOutputHeader.c_str(), mOutputHeader.length());

    if(bytesWritten < 0)
    {
    	return -1;
    }

    totalBytesWritten += bytesWritten;

    bytesWritten = write(sockFd,mOutputData.c_str(), mOutputData.length());

    totalBytesWritten += bytesWritten;

    return totalBytesWritten;
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

