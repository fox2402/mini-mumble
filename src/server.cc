#include "server.h"

#include <iostream>
#include <system_error>

#include <netinet/ip.h>
#include <unistd.h>

Server::Server(const Options& opt)
{
  struct sockaddr_in info;
  if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
  {
    throw std::system_error(server_socket,
    std::system_category(), "Cannot create socket");    
  }
  else
  {
    std::cout << "Socket OK" << std::endl;
  }
  info.sin_family = AF_INET;
  info.sin_addr.s_addr = htonl(INADDR_ANY);
  info.sin_port = htons(opt.port);

  bind_socket(&info); 
}

Server::~Server()
{
  close(server_socket);
}

void Server::bind_socket(struct sockaddr_in* info)
{
  if ((bind(server_socket, (struct sockaddr*)&info,
    sizeof(struct sockaddr_in))) == -1)
  {
    throw std::system_error(server_socket,
    std::system_category(),"Cannot bind socket");
  }
  else
  {
    std::cout << "Binding OK" << std::endl;
  }
}

