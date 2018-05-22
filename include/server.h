#pragma once

#include "options.h"

#include <vector>
#include <stdexcept>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>

class Server
{
  public:
    Server(const Options& opt);
    ~Server();  
    
    void begin_listen();

    
  private:
    void bind_socket(const Options& opt);
    
    void manage_req();

    
    std::vector<int>  clients_list;
    int               server_socket;
    char*             buffer;
    std::string       password;

};
