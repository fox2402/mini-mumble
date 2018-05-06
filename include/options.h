#pragma once

#include <string>

struct Options
{
  Options();
  Options(char** argv, int argc);
  bool                silent    {false};
  int                 is_server {-1};
  int                 port      {-1};
  std::string         addr      {"None"};
  std::string         login     {"defaultuser"};
  std::string         pass      {""};
};
