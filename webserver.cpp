/* A simple server in the internet domain using TCP
   The port number is 2468 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "HttpResponder.h"

#define SERVER_PORT 2468 // static server port
#define MAX_QUEUED_CONNECTIONS 5 // number of concurrent connections

using namespace std;

int main(int argc, char *argv[])
{

    //Declare networking variables
    int socketFd, newSocket; 
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientLength;
    fd_set activeFdSet;
    string output;

    cout << "Starting web server..." << endl;
    //set server address
    memset(&serverAddr, 0, sizeof(serverAddr)); //clear
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    HttpResponder hr;



    //create, bind, listen to socket and check for errors
    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socketFd < 0)
    {
        cout << "Error creating socket. Exiting." << endl;
        return -1;
    }

    if (bind(socketFd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr_in)) < 0)
    {
        cout << "Error binding. Exiting." << endl;
        return -1;
    }

    if (listen(socketFd,MAX_QUEUED_CONNECTIONS) < 0)
    {
        cout << "Error listening. Exiting." << endl;
        return -1;
    }

    //Manage activeFdSet 
    FD_ZERO(&activeFdSet);
    FD_SET(socketFd, &activeFdSet);

    //main loop
    cout << "Web server started." << endl;
    cout << "Accepting HTTP connections on port "<< SERVER_PORT << "..." << endl << endl;
    while(1)
    {
        if(select(socketFd + 1, &activeFdSet, NULL, NULL, NULL) < 0)
        {
            cout << "Socket error. Exiting." << endl;
            exit(-1);
        }

        if(FD_ISSET(socketFd, &activeFdSet))
        {
            //Get and set new socket in the active FD set.

            newSocket = accept(socketFd, (struct sockaddr *) &clientAddr, &clientLength);

            if (newSocket < 0)
            {
                cout << "Error accepting. Exiting." << endl;
                return -1;
            }

            FD_SET(newSocket, &activeFdSet);
        }

        if(FD_ISSET(newSocket, &activeFdSet))
        {
            hr.readRequest(newSocket);
            hr.logRequestToConsole();
            hr.writeOnTCP(newSocket);
            close(newSocket);
        }
    }

    close(socketFd);
    return 0;
}