#ifndef __HTTP_RESPONDER_H__ 
#define __HTTP_RESPONDER_H__ 

#include <string>

class HttpResponder {
	
	private:
		static const int BYTE_READ_SIZE = 256; //read 256 bytes at a type

	public:

		//Member Variables
		std::string mRequestData;

		//Member Functions
		HttpResponder();

		//Recursively read all bytes socket and save it in member variable
		ssize_t readRequest(int sockFd);

		//Print the request to console
		void logRequestToConsole();

		int writeOnTCP(int sockFd);
};


#endif