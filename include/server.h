#pragma once

#include "options.h"

#include <vector>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>

class Server
{
  public:
    Server(const Options& opt);
    ~Server();  

  private:
    void bind_socket(const Options& opt);

    std::vector<int>  clients_list;
    int               server_socket;


};
