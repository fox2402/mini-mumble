#include "server.h"

#include <iostream>
#include <system_error>
#include <algorithm>

#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

namespace
{
  const int max_events = 50;
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

  void setnonblocking(int fd)
  {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
      throw std::system_error(errno, std::system_category());
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
      throw std::system_error(errno, std::system_category());
  }

}

Server::Server(const Options& opt)
{
  buffer = new char[1024];
  bind_socket(opt);
  password = opt.pass;
  is_password = password.length() > 0;
  ep_socket = epoll_create1(0);

  if (ep_socket == -1)
    throw std::system_error(errno, std::system_category());

}

Server::~Server()
{
  delete[] buffer;
  close(server_socket);
  close(ep_socket);
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

void Server::server_loop()
{
  struct epoll_event events[max_events];
  struct epoll_event ev;
  for(;;)
  {
    int nfds = epoll_wait(ep_socket, events, max_events, -1);
    if (nfds == -1)
      throw std::system_error(errno, std::system_category());
    for (int i = 0; i < nfds; i++)
    {
      if (events[i].data.fd == server_socket)
      {
        int client = accept(server_socket, NULL, NULL);
        std::cout << "Accepted one client" << std::endl;
        if (client == -1)
          throw std::system_error(errno, std::system_category());
        setnonblocking(client);
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = client;
        if (epoll_ctl(ep_socket, EPOLL_CTL_ADD, client, &ev) == -1)
          throw std::system_error(errno, std::system_category(),
            "can't add to epoll");
      }
      else
      {
          std::cout << "Reading one client" << std::endl;
          read_size = read(events[i].data.fd, buffer, 1024);
          std::cout << "Managing one client" << std::endl;
          std::cout << "Buffer: " << buffer << std::endl;
          
          if (read_size == 0)
          {
            std::cout << "Lost one client" << std::endl;
            if (epoll_ctl(ep_socket, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1)
              throw std::system_error(errno, std::system_category(),
              "can't add to epoll");
          }
          manage_req(events[i].data.fd);
      }
    }
    memset(buffer, 0, 1024);
  }
}

void Server::begin_listen()
{
  int i;
  if ((i = listen(server_socket, 50)) == -1)
    throw std::system_error(errno, std::system_category(), "cannot listen");
  std::cout << "Listening..." << std::endl;
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = server_socket;
  if (epoll_ctl(ep_socket, EPOLL_CTL_ADD, server_socket, &ev) == -1)
    throw std::system_error(errno, std::system_category(), "can't add to epoll");
  server_loop();
}

void Server::manage_req(int client)
{
  std::string s(buffer, 3);
  if (!s.compare("CON"))
  {
    manage_CON(client);
    return;
  }
  if (!s.compare("PWD"))
  {
    manage_PWD(client);
    return;
  }
  
  if (!check_valid_client(client))
  {
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
  if (!s.compare("ACK"))
  {
    manage_ACK(client);
    return;
  }
}


bool Server::check_valid_client(int socket)
{
  auto it = client_login_map.find(socket);
  if (it != client_login_map.end())
  {
    return std::get<1>(std::get<1>(*it));
  }
  return false;

}


void Server::manage_CON(int client)
{
  
  client_login_map.emplace(std::make_pair(client, std::make_pair(
  std::string(buffer).substr(0, 3),false)));
  if (is_password)
  {
    write(client, "PWD", 3);
  }
  else
  {
    std::get<1>(client_login_map[client]) = true;
    write(client, "ACK", 3);
  }
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
      std::get<1>(client_login_map[client]) = true;
      return;
    }
  }
  write_size = write(client, "NOP", 3);
}

void Server::manage_ACK(int client)
{
  (void) client;
  return;
}
