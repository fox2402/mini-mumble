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
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(std::stoi(opt.port));
  inet_pton(AF_INET, opt.addr.c_str(), &serv_addr.sin_addr.s_addr);
}

Client::~Client()
{
  delete[] buffer;
}

void Client::connect_to_server()
{
  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw std::system_error(errno, std::system_category(), "Failed to create socket");
  if (connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    throw std::system_error(0, std::system_category(), "Failed to connect to server");
}

void Client::communicate()
{
  while(true)
  {
    memset(buffer, 0, 1024);
    std::string s;
    std::cin >> s;
    send(sfd, s.c_str(), strlen(s.c_str()), 0);
    read(sfd, buffer, 1024);
    for(size_t i = 0; i < strlen(buffer); i++)
    {
      std::cout << buffer[i];
    }
    std::cout << std::endl;
  }
}
