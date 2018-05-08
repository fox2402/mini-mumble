#pragma once

#include<stdexcept>
#include<iostream>
#include<string>

struct Options
{
  Options();
  Options(int argc, char** argv);
  bool                verbose   {false};
  bool                silent    {false};
  int                 is_server {-1};
  int                 port      {-1};
  std::string         addr      {"None"};
  std::string         login     {"defaultuser"};
  std::string         pass      {""};
  void complete();
};

std::ostream& operator<<(std::ostream& os, Options& o);
