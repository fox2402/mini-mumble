#pragma once

#include "options.h"

#include <vector>
#include <stdexcept>

#include<netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class Client
{
  public:
    Client(const Options& opt);
    ~Client();  
    void connect_to_server();
    
  private:
    char*               buffer;
    struct sockaddr_in  serv_addr;
    int                 sfd;
};
