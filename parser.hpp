#if __cplusplus
#include <string>

// Declaration of data types
// HTTP Request Types
typedef enum HTTP_RequestTypes_t
{
  HTTP_REQ_PUT    = 0,
  HTTP_REQ_GET,
  HTTP_REQ_HEAD,
  HTTP_REQ_DELETE,
  HTTP_REQ_NA     /*default*/
}HTTP_RequestTypes_t;

// HTTP Version
typedef enum HTTP_Version_t
{
  HTTP_V_1_0,
  HTTP_V_1_1 /*default*/
}HTTP_Version_t;


class ClientRequestMsgDecode {
public:
  // constructor
  ClientRequestMsgDecode (char * Msg); 

private:
  void GetNextToken 
    (char * Dest, char ** Source, const char * SourceEnd);

public:
  HTTP_RequestTypes_t HTTP_Request;
  HTTP_Version_t      HTTP_Version;
  std::string         RequestedFileLocation;
};

#endif

