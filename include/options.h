#pragma once

#include<stdexcept>
#include<iostream>
#include<string>

struct Options
{
  Options();
  void init(int argc, char** argv);
  bool                verbose   {false};
  bool                silent    {false};
  int                 is_server {-1};
  std::string         port      {"None"};
  std::string         addr      {"None"};
  std::string         login     {"defaultuser"};
  std::string         pass      {""};
  void complete();
};

std::ostream& operator<<(std::ostream& os, Options& o);
