#pragma once

#include <string>

struct Options
{
  Options();
  Options(char** argv, int argc);
  int                 is_server {-1};
  int                 port      {-1};
  std::string         addr      {"None"};
};
