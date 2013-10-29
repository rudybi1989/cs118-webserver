#ifndef __HTTP_PACKET_GENERATOR_H__ 
#define __HTTP_PACKET_GENERATOR_H__ 

#include <vector>
#include <string.h>

class HttpPacketGenerator {

public:
	std::string requestData;

	HttpPacketGenerator(){
		requestData.clear();
	};

	//Recursively read all bytes socket and save it in member variable
	ssize_t readRequest(int sockFd){

		char* tempBuffer;
		ssize_t bytesRead = 0;
		ssize_t totalBytesRead = 0;


		bytesRead = read(sockFd, tempBuffer, 256); //read 256 bytes at a time.

		if (bytesRead < 0)
		{
			return -1; //read error
		}

		requestData.clear();
		requestData.append(tempBuffer);

		return totalBytesRead;
	}; 

	//Print the request to console
	void logRequestToConsole()
	{
		std::cout << requestData << std::endl;

		return 
	};

	int writeOnTCP(){};

};


#endif