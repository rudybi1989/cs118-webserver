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
		int mSockFd;



		int readAndWriteData(std::string FileLocation);
		int getFileSize(std::string fileLocation); //get file size in bytes or -1 if file does not exist on server
		std::string getContentType(std::string fileLocation);



	public:

		//Member Functions
		HttpResponder();

		//Recursively read all bytes socket and save it in member variable
		ssize_t readRequest(int sockFd);

		//Print the request to console
		void logRequestToConsole();


		int processRequest();

                
};


#endif
