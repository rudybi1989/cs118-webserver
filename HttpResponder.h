#ifndef __HTTP_RESPONDER_H__ 
#define __HTTP_RESPONDER_H__ 

#include <string>

class HttpResponder {
	
	private:
		static const int BYTE_READ_SIZE = 2048; //read 2048 bytes at a time
		//Member Variables
		std::string mRequestData;
		std::string mOutputHeader;
		std::string mOutputData;



		int sendHeader(int sockFd);


	public:

		//Member Functions
		HttpResponder();

		//Recursively read all bytes socket and save it in member variable
		ssize_t readRequest(int sockFd);

		//Print the request to console
		void logRequestToConsole();

		void generatePacket();

		void processRequest();

		int writeOnTCP(int sockFd);
                
                void ReadAndWriteBinary(std::string FileLocation);
};


#endif
