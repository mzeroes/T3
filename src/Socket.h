#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "T3.hpp"

// TODO! refactor to add common thing to this file 
class Socket
{
protected:
  // File Discriptor for 1st and 2nd party
  int sockFD, newsockFD; 
  int bindFD, listenFD;  
  // CONNECTION PORT
  int port_no;
  int bytes_sent, bytes_received;

  struct sockaddr_in serv_addr;
  

public:
  Socket(int PORT);
  Socket(Socket &&);
  Socket &operator=(Socket &&);
  Socket(Socket const &) = delete;
  Socket &operator=(Socket const &) = delete;

  virtual bool SendMessage(std::string const &buffer);
  virtual bool GetMessage(std::string &buffer);
  virtual ~Socket();
};
