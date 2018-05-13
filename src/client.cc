#include "client.h"

#include <iostream>
#include <system_error>

#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

Client::Client(const Options& opt)
{
  buffer = new char[1024];
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sfd < 0)
    throw std::system_error(errno, std::system_category(), "Failed to create socket");
  memset(&addr, '0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(std::stoi(opt.port));
  inet_pton(AF_INET, opt.addr.c_str(), &addr.sin_addr.s_addr);
  if (connect(sfd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    throw std::system_error(0, std::system_category(), "Failed to connect to server");
  send(sfd, "Hi\n", 400, 0);
  read(sfd, buffer, 1024);
  for(size_t i = 0; i < strlen(buffer); i++)
  {
    std::cout << buffer[i];
  }
  std::cout << std::endl;
}

Client::~Client()
{
  delete[] buffer;
}

