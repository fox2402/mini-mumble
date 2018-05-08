#include "options.h"

Options::Options()
{
  //FIXME
}

void Options::init(int argc, char** argv)
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
    else if(!s.compare("-v"))
      verbose = true;
    else
      throw std::invalid_argument(s.append(": invalid argument"));
  }
}

void Options::complete()
{
  while(port == -1)
  {
    std::cout << "Enter port:" << std::endl;
    try
    {
      std::string s;
      std::cin >> s;
      port = std::stoi(s);
      std::cout << std::endl;
    }
    catch(const std::invalid_argument& e)
    {
      std::cerr << "Invalid port number" << std::endl;
      std::cout << std::endl;
    }
  }
  if(is_server == -1)
  {
    if(!addr.compare("None"))
    {
      std::cout << "Enter address:" << std::endl;
      std::cin >> addr; 
      std::cout << std::endl;
    }
    if(!login.compare("defaultuser"))
    {
      std::cout << "Enter login:" << std::endl;
      std::cin >> login; 
      std::cout << std::endl;
    }
  }
}

std::ostream& operator<<(std::ostream& os, Options& o)
{
  os << "Run as " << ((o.is_server == 1) ? "server" : "client") << std::endl
    << "Port: " << o.port << std::endl
    << "Address: " << o.addr << std::endl
    << "Login: " << o.login << std::endl
    << "Password: " << o.pass << std::endl
    << ((o.silent) ? "Silent" : "Not silent") << std::endl
    << ((o.verbose) ? "Verbose" : "Non-verbose") << std::endl;
  return os;
}
