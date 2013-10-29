#include "parser.hpp"
#include <string>
#include <iostream>
#include <string.h>
using namespace std;

 // constructor
ClientRequestMsgDecode::ClientRequestMsgDecode (char * Msg) 
{
  /*ClientRequestMsg = std::string (Msg);
    std::regex e("(GET|PUT|HEAD|DELETE)\\s(.*)", std::regex_constants::extended);
    std::smatch m;
    std::regex_match (ClientRequestMsg, m, e);
    for (unsigned int i=0; i < m.size(); i++)
      std::cout << m[i] << std::endl;*/


  int len = strlen(Msg), i;
  /*Default values*/
  HTTP_Request = HTTP_REQ_NA;
  RequestedFileLocation.assign("");
  HTTP_Version = HTTP_V_1_1;
  HTTP_ConnectionType = CONNECTION_CLOSE;
  if (len == 0) return;
  if (Msg == 0) return;  
  char *Source = Msg;
  const char *SourceEnd = Msg + len;
  char *Request = new char [len];
  char *FileLocation = new char [len];
  char *HTTPVersion = new char [len];
  /////////////////////////////////////////////////////////////////////////////
  // Request type = "GET" or "PUT" or "HEAD"
  GetNextToken (Request, &Source, SourceEnd);
  if (strcmp(Request,"GET") == 0)
    HTTP_Request = HTTP_REQ_GET;
  else if (strcmp(Request, "PUT") == 0)
    HTTP_Request = HTTP_REQ_PUT;
  else if (strcmp(Request, "POST") == 0)
    HTTP_Request = HTTP_REQ_POST;
  else if (strcmp (Request, "HEAD") == 0)
    HTTP_Request = HTTP_REQ_HEAD;
  else if (strcmp (Request, "DELETE") == 0)
    HTTP_Request = HTTP_REQ_DELETE;
  else HTTP_Request = HTTP_REQ_NA;

  /////////////////////////////////////////////////////////////////////////////
  //FileLocation
  GetNextToken (FileLocation, &Source, SourceEnd); 
  RequestedFileLocation.assign(FileLocation);

  /////////////////////////////////////////////////////////////////////////////
  // HTTP version = "1.0" or "1.1"
  GetNextToken (HTTPVersion, &Source, SourceEnd);
  
  if (strcmp (HTTPVersion, "HTTP/1.1")==0) 
    HTTP_Version = HTTP_V_1_0;
  else if (strcmp (HTTPVersion, "HTTP/1.0") == 0)
    HTTP_Version = HTTP_V_1_1;
  else
    HTTP_Version = HTTP_V_1_1;

  /////////////////////////////////////////////////////////////////////////////
  // Connection Type = "keep-alive" or "close"
  char * ConnectionType = strstr (Source, "Connection: ");
  if (ConnectionType != 0)
  {
    if (strstr(ConnectionType, "Connection: keep-alive"))
      HTTP_ConnectionType = CONNECTION_KEEP_ALIVE;
  }
  cout << "**Parser Output**:" <<endl;
  cout << "FileLocation: " << FileLocation << endl;
  cout << "Req:" << HTTP_Request << " =" <<Request << endl;
  cout << "Ver:" << HTTP_Version << " =" << HTTPVersion << endl;
  cout << "ConnType:"<< HTTP_ConnectionType << "=\"" << ConnectionType << "\""<< endl; 
  
  // Free the buffers.
  delete [] Request;
  delete [] FileLocation;
  delete [] HTTPVersion;

}

void ClientRequestMsgDecode::GetNextToken 
    (char * Dest, char ** Source, const char * SourceEnd)
{
  for ( ; ;)
  {
    if ( Source != 0 && *Source != 0 && *Source != SourceEnd )
    {
      if (**Source == ' ' || **Source == '\n' || **Source == '\r' )
      {
        *Source = *Source + (int)1;
        break;
      }
      else
      {
        *Dest = **Source;
        Dest++;
        *Dest = '\0';
        *Source = *Source + (int)1; //*Source++ increments by 8????
      }
    }
    else
       break;
  }
}
