#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include "HttpResponder.h"
#include "parser.hpp"


using namespace std;

HttpResponder::HttpResponder(){
	mRequestData.clear();
}

ssize_t HttpResponder::readRequest(int sockFd){

	char tempBuffer[HttpResponder::BYTE_READ_SIZE];
	memset(&tempBuffer, '0', sizeof(tempBuffer));
	ssize_t bytesRead = 0;
	ssize_t totalBytesRead = 0;
	mSockFd = sockFd;
	
	bytesRead = read(mSockFd, tempBuffer, HttpResponder::BYTE_READ_SIZE); //read 2048 bytes at a time.
		
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

int HttpResponder::processRequest() {

	//default HTTP Response Parameters
	std::string httpVersion = "HTTP/1.1";
	std::string httpStatus = "200 OK";
	std::string contentType = "text/HTML";
	std::string CRLF = "\r\n";
	std::string rootDir = "/";
	int requestedContentSize = 0;
	std::string fileLocation = "";

	mOutputHeader.clear();
	mOutputData.clear();

	ClientRequestMsgDecode msgDecoder((char*) mRequestData.c_str()); //parse message
	
	if (strcmp(msgDecoder.RequestedFileLocation.c_str(), rootDir.c_str()) == 0)
	{
		fileLocation = "index.html";
	}

	else
	{
		fileLocation = msgDecoder.RequestedFileLocation.erase(0,1); //erase the initial "/" in the requested file location
	}

	if(msgDecoder.HTTP_Request != HTTP_REQ_GET) //Server only supports GET requests
	{
		httpStatus = "501 Not Implemented"; 
		fileLocation = "static/501.html";
	}

	if(!msgDecoder.EndSignatureFound) //the HTTP request message was not valid
	{
		httpStatus = "400 Bad Request"; 
		fileLocation = "static/400.html";
	}

	requestedContentSize = getFileSize(fileLocation);

	if(requestedContentSize < 0) //file does not exist
	{
		httpStatus = "404 File Not Found";
		fileLocation = "static/404.html";
		requestedContentSize = getFileSize(fileLocation); //get file size of 404 HTML page now
	}

	contentType = getContentType(fileLocation);

	mOutputHeader.append(httpVersion + " " + httpStatus + CRLF);
	mOutputHeader.append("Content-Type: " + contentType + CRLF);

	if(requestedContentSize > 0)
	{
		std::stringstream contentSize_ss;
		contentSize_ss << requestedContentSize;
		mOutputHeader.append("Content-Length: " + contentSize_ss.str() + CRLF);
	}

	mOutputHeader.append("Connection: close" + CRLF); //always close connection
    mOutputHeader.append(CRLF); //end message

    cout << "File Location is: " << fileLocation << endl;
    cout << "File size is: " << requestedContentSize << endl;
    cout << "Content Type is: " << contentType << endl;
   
    //write head
    int bytesWritten = write(mSockFd, mOutputHeader.c_str(), mOutputHeader.length());

    if(bytesWritten < 0)
    {
    	return -1;
    }

    if(requestedContentSize > 0)
    {
    //Write Data
	  char *buffer = new char[requestedContentSize];
	  int blockSize = requestedContentSize;
	  int bytesRemaining = 0;
	  int bytesToRead = 0;

	  ifstream inStream(fileLocation.c_str(), std::ios::in | std::ios::binary);
	  if(inStream.good()) 
	  {
	    
	    int length = requestedContentSize;

	    bytesRemaining = length;

	    while (bytesRemaining > 0 && inStream)
	    {

	      if (blockSize > bytesToRead)
	        bytesToRead = length;
	      else
	        bytesToRead = blockSize;

	      inStream.read(buffer, bytesToRead);

	      bytesRemaining -= bytesToRead;
	      write(mSockFd, buffer, bytesToRead);
	    }

	    inStream.close();
	    delete[] buffer;
	  }

	  else
	  {
	    return -1;
	  }
	}

	return 1;
};

std::string HttpResponder::getContentType(std::string fileLocation)
{
	if(fileLocation.find(".gif") != std::string::npos || fileLocation.find(".GIF") != std::string::npos)
		return "image/gif";

	if(fileLocation.find(".html") != std::string::npos)
		return "text/html";

	if(fileLocation.find(".txt") != std::string::npos)
		return "text/plain";

	if(fileLocation.find(".png") != std::string::npos || fileLocation.find(".PNG") != std::string::npos)
		return "image/png";

	if(fileLocation.find(".jpg") != std::string::npos || fileLocation.find(".jpeg") != std::string::npos)
		return "image/jpeg";

	if(fileLocation.find(".pdf") != std::string::npos || fileLocation.find(".PDF") != std::string::npos)
		return "application/pdf";


	return "application/octet-stream"; //generic byte stream download

}

int HttpResponder::getFileSize(std::string fileLocation)
{
	struct stat fileStats;

    if(stat(fileLocation.c_str(), &fileStats) != -1)
    	return fileStats.st_size;
  	
  	else
  		return -1;
}



int HttpResponder::readAndWriteData(string FileLocation)
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

  	int totalBytesWritten = 0;
    int bytesWritten = 0;

    bytesWritten = write(mSockFd, mOutputHeader.c_str(), mOutputHeader.length());

    if(bytesWritten < 0)
    {
    	return -1;
    }

    totalBytesWritten += bytesWritten;

    bytesWritten = write(mSockFd,mOutputData.c_str(), mOutputData.length());

    totalBytesWritten += bytesWritten;

    return totalBytesWritten;
}

