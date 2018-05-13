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
    
  private:
    char*             buffer;
    struct sockaddr_in addr;
};
