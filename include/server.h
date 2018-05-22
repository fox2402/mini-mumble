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
     
    void manage_req(int client);
    
    void manage_CON(int client);
    void manage_VOI(int client);
    void manage_CHA(int client);
    void manage_DCT(int client);
    void manage_PWD(int client);
    void manage_ACK(int client);
    
    std::vector<int>  clients_list;
    int               server_socket;
    char*             buffer;
    std::string       password;

    bool              is_password{false};

};
