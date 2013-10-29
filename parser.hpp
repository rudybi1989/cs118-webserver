#if __cplusplus
#include <string>

// Declaration of data types
// HTTP Request Types
typedef enum HTTP_RequestType_t
{
  HTTP_REQ_PUT    = 0,
  HTTP_REQ_GET,
  HTTP_REQ_HEAD,
  HTTP_REQ_DELETE,
  HTTP_REQ_POST,
  HTTP_REQ_NA     /*default*/
}HTTP_RequestType_t;

// HTTP Version
typedef enum HTTP_Version_t
{
  HTTP_V_1_0,
  HTTP_V_1_1 /*default*/
}HTTP_Version_t;

typedef enum HTTP_Connection_t
{
  CONNECTION_KEEP_ALIVE,
  CONNECTION_CLOSE /*default*/ 
}HTTP_Connection_t;

class ClientRequestMsgDecode {
public:
  // constructor
  ClientRequestMsgDecode (char * Msg); 

private:
  void GetNextToken 
    (char * Dest, char ** Source, const char * SourceEnd);

public:
  HTTP_RequestType_t HTTP_Request; /*GET, PUT, etc */
  HTTP_Version_t     HTTP_Version; /*1.0 or 1.1 */
  std::string        RequestedFileLocation;
  HTTP_Connection_t  HTTP_ConnectionType; /* keep-alive or closed */
};

#endif

