#CS118 Webserver Project

#Server Design

The webserver is created in C++, utilizing the BSD socket library. It runs on a static port 2468.
The webserver only supports GET HTTP Requests. It will respond with a 405 Method Not Allowed error if using another HTTP request method. Per the project specs, the web server supports HTML, JPEG, and GIF file types. It also supports PNG, PDF, plain text, and application/octet-streaming types. It will respond with a 404 File Not Found error if the resource was not found on the webserver.

The server is architected with three major components: 
1. webserver.cpp
2. HttpResponder.cpp
3. parser.cpp (ClientRequestMsgDecode class)
An overview of architecture can be seen in the following diagram:
Figure 1: Server Architecture
￼
The main loop for socket creation, binding, listening, accepting, and closing connections occurs in the main function of webserver.cpp. The main loop uses the select() function to return an active set of sockets to read and process. It also utilizes the helper class HttpResponder to read the socket HTTP request, log the request to the console, and process the request.

The HttpResponder class creates the server response message and adds data in the HttpResponder::processRequest() method. This method relies on the ClientRequestMsgDecode class defined in parser.cpp to parse the HTTP request message from the client. The Parser class will populate fields such as the request type (GET, POST, PUT, DELETE), HTTP Version, Requested File, and HTTP Connection Type. The HttpResponder will use these parameters to generate the HTTP header, find the desired resource on the server, and write the header and data in chunks back to the client.
Control is then returned to the main loop, which will close the read socket and loop back to the select() function to repeat the process.

#Difficulties
The main difficulty for our group was determining the best way to split larger files into smaller chunks. Initially we tried to dynamically allocate buffers based on the content size; however, running on the virtual Ubuntu image caused insufficient memory errors on larger files. We ultimately settled on interleaving read/write operations such that we allocated a fixed 2kB buffer size to read and subsequently
write to the TCP socket, repeating until all the data was successfully written into the client and then closing the stream and socket.

#Compiling and Running
To compile and run the webserver:
1. Run “make” command from the project root. This will compile the project
using g++.
2. Start webserver by running command “./webserver”. No arguments are
needed.
3. If socket creation, bind, and listen functions are successful, the webserver
will be listening on port 2468 on localhost.
4. Accessing http://localhost:2468 will return the index.html file located at
/static/index.html on the webserver.

#Sample Outputs
When sending the GET request the root directory, such as:

GET / HTTP/1.1
Host: localhost:2468
Content-Type: application/x-www-form-urlencoded
Accept: */*
Cookie: csrftoken=wXbPLf5Wy78FnjntUtZLAzlyI2sZdMKQ
User-Agent: CocoaRestClient/10 CFNetwork/673.0.3 Darwin/13.0.0 (x86_64) (MacBookAir5%2C2)
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: keep-alive
The response message from the server is:
￼
HTTP/1.1 200 OK
Content-Type: text/html
Date: Thu 30 Oct 2013 04:54:03 GMT
Content-Length: 743
Connection: closed

The response header is paired with /static/index.html page on the webserver, which serves as the homepage to our webserver. The next figure shows the marked up HTML of the /static/index.html page:
￼From the image, you can see that the webserver can GET HTML, as well as JPEG and GIF files. Other supported formats are PDF, PNG, and plain/text. Other images can be viewed by manually GET-ing images in the /image folder in the webserver root.
In the case that a GET request is issued but, the resource cannot be found:

GET /nonexistant.html HTTP/1.1
Host: localhost:2468
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp, */*;q=0.8
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36
Accept-Encoding: gzip,deflate,sdch
Accept-Language: en-US,en;q=0.8

The server responds with:
HTTP/1.1 404 Not Found

Content-Type: text/html
Date: Thu 31 Oct 2013 05:01:45 GMT
Content-Length: 41
Connection: closed
The corresponding HTML data is:
￼
