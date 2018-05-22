#include "server.h"

#include <iostream>
#include <system_error>
#include <algorithm>

#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

namespace
{
  void init_hints(struct addrinfo* hints)
  {
      memset(hints, 0, sizeof(struct addrinfo));
      hints->ai_family = AF_UNSPEC;
      hints->ai_socktype = SOCK_STREAM;
      hints->ai_flags = AI_PASSIVE;
      hints->ai_protocol = 0;
      hints->ai_canonname = NULL;
      hints->ai_addr = NULL;
      hints->ai_next = NULL;
  }
}

Server::Server(const Options& opt)
{
  buffer = new char[1024];
  bind_socket(opt);
  password = opt.pass;
  is_password = password.length() > 0;
}

Server::~Server()
{
  delete[] buffer;
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
    throw std::system_error(errno, std::system_category(), "cannot retrieve\
addr");
  }
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue;
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break;
    close(sfd);
  }
  freeaddrinfo(result);
  if (rp == NULL)
  {
    throw std::system_error(0, std::system_category(), "Failed to find an\
addr");
  }
  server_socket = sfd;

  std::cout << "Socket is up and binded" << std::endl;
}

void Server::begin_listen()
{
  int i;
  if ((i = listen(server_socket, 50)) == -1)
    throw std::system_error(errno, std::system_category(), "cannot listen");
  std::cout << "Listening..." << std::endl;
  int client = accept(server_socket, NULL, NULL);
  std::cout << "Accepted one client" << std::endl;
  while(true)
  {
    memset(buffer, 0, 1024);
    read_size = read(client, buffer, 1024);
    if (!read_size)
    {
      std::cout << "End of com, shutting down" << std::endl;
      break;
    }
    write (client, "Server:", 7);
    ssize_t i = write (client, buffer, read_size);
    if (i == EPIPE)
    {
      std::cout << "Client Lost" << std::endl;
      close(client);
    }
  }
}

void Server::manage_req(int client)
{
  std::string s(buffer, 3);
  if (!s.compare("CON"))
  {
    manage_CON(client);
    return;
  }
  if (!s.compare("VOI"))
  {
    manage_VOI(client);
    return;
  }
  if (!s.compare("CHA"))
  {
    manage_CHA(client);
    return;
  }
  if (!s.compare("DCT"))
  {
    manage_DCT(client);
    return;
  }
  if (!s.compare("PWD"))
  {
    manage_PWD(client);
    return;
  }
  if (!s.compare("ACK"))
  {
    manage_ACK(client);
    return;
  }
}

void Server::manage_CON(int client)
{
  if (is_password)
    write(client, "PWD", 3);
  else
    write(client, "ACK", 3);
}

void Server::manage_VOI(int client)
{
  for (const int& cli : clients_list)
  {
    if (cli != client)
      write(cli, buffer, read_size);
  }
}

void Server::manage_CHA(int client)
{
  for (const int& cli : clients_list)
  {
    if (cli != client)
      write(cli, buffer, read_size);
  } 
}


void Server::manage_DCT(int client)
{
  close(client);
  auto itr = std::find(clients_list.begin(), clients_list.end(), client);
  clients_list.erase(itr);
}

void Server::manage_PWD(int client)
{
  if (read_size > 3)
  {
    std::string str(buffer + 3, read_size - 3);
    if (!str.compare(password))
    {
      write(client, "ACK", 3);
      return;
    }
  }
  write(client, "NOP", 3);
}

void Server::manage_ACK(int client)
{
  (void) client;
  return;
}


