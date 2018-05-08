#include "server.h"

#include <iostream>
#include <system_error>

#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
namespace
{
  void init_hints(struct addrinfo* hints)
  {
      memset(hints, 0, sizeof(struct addrinfo));
      hints->ai_family = AF_UNSPEC;
      hints->ai_socktype = SOCK_DGRAM;
      hints->ai_flags = AI_PASSIVE;
      hints->ai_protocol = 0;
      hints->ai_canonname = NULL;
      hints->ai_addr = NULL;
      hints->ai_next = NULL;
  }
}

Server::Server(const Options& opt)
{
  bind_socket(opt); 
}

Server::~Server()
{
  close(server_socket);
}

void Server::bind_socket(const Options& opt)
{
  struct addrinfo hints;
  struct addrinfo* result;
  struct addrinfo* rp;
  int sfd;
  init_hints(&hints);

  int s = getaddrinfo(NULL, opt.port.c_str(), &hints, &result);
  if (s != 0)
  {
    throw std::system_error(s, std::system_category(), "cannot retrieve addr");
  }
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue;
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
  }
  if (rp == NULL)
  {
    throw std::system_error(0, std::system_category(), "Failed to find an\
    addr");
  }
  server_socket = sfd;

  std::cout << "Socket is up and binded" << std::endl;
}

