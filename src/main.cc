//system includes do not have .h
#include <iostream>
#include <vector>

#include "options.h"
#include "sample.h"
#include "server.h"


int main(int argc, char** argv)
{
  struct Options o;
  try
  {
    struct Options o = Options(argc, argv);
    std::cout << o;
  }
  catch(const std::invalid_argument& e)
  {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  o.complete();
  std::cout << o;
  if (o.server == 1)
  {
    try
    {
      Server s(o);
    }
    catch(const std::system_error& e)
    {
      std::cerr << e.what() << std::endl;
      return -1;
    }
  }
  return 0;
}
