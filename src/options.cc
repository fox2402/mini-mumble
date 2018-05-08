#include "options.h"

Options::Options()
{
  //FIXME
}

Options::Options(int argc, char** argv)
{
  for(int i = 1; i < argc; i++)
  {
    std::string s(argv[i]);
    if(!s.compare("-s"))
      is_server = 1;
    else if(!s.compare("-p"))
    {
      if(i + 1 >= argc)
        throw std::invalid_argument("missing argument for -p");
      try
      {
        port = std::stoi(argv[i + 1]);
      }
      catch(const std::invalid_argument& e)
      {
        throw std::invalid_argument("invalid argument for -p");
      }
      i++;
    }
    else if(!s.compare("-a"))
    {
      if(i + 1 >= argc)
        throw std::invalid_argument("missing argument for -a");
      addr = std::string(argv[i + 1]);
      i++; 
    }
    else if(!s.compare("-l"))
    {
      if(i + 1 >= argc)
        throw std::invalid_argument("missing argument for -l");
      login = std::string(argv[i + 1]);
      i++;
    }
    else if(!s.compare("-m"))
    {
      if(i + 1 >= argc)
        throw std::invalid_argument("missing argument for -m");
      pass = std::string(argv[i + 1]);
      i++;
    }
    else if(!s.compare("-n"))
      silent = true;
    else
      throw std::invalid_argument(s.append(": invalid argument"));
  }
}
