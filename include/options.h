#pragma once

#include <string>

struct Options
{
  Options();

  bool                is_server;
  int                 port;
  std::string         addr;
};
