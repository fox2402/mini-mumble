#pragma once

#include "options.h"

#include <vector>
#include <stdexcept>
#include <string>
#include <map>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

class Server
{
  public:
    Server(const Options& opt);
    ~Server();  
    
    void begin_listen();

    
  private:
    bool check_valid_client(int socket);


    void bind_socket(const Options& opt);
    
    void server_loop();

    void manage_req(int client);
    
    void manage_CON(int client);
    void manage_VOI(int client);
    void manage_CHA(int client);
    void manage_DCT(int client);
    void manage_PWD(int client);
    void manage_ACK(int client);
    

    std::map<int, std::pair<std::string, bool>> client_login_map;
    std::vector<int>  clients_list;
    int               server_socket;
    int               ep_socket;
    
    char*             buffer;
    std::string       password;
    ssize_t           read_size;
    ssize_t           write_size{0};

    bool              is_password{false};

};
