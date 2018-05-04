//system includes do not have .h
#include <iostream>
#include <vector>
#include "sample.h"

int main()
{
  //by default, all system include are in "std" namespace
  //cout is the standard output, it's a stream so you can put things in it with
  //"<<" it will print it the best way it can (if a defined conversion exists)
  //std::endl is just a '\n' for most system 
  std::cout << "Hello World!" << std::endl;
  
  //As class can be templated, meaning they can be generic for different types
  //each templated class can be seen as a different "namespace" so if you want
  //a vector of int you'll type this note the "new" keyword which construct
  //your object on the heap, as a malloc would. the free equivalent is delete.
  //never use malloc in c++ except for very low level black magic things.
  std::vector<int>* v1 = new std::vector<int>();

  //scopes are very powerful in c++. They do the same things as in c, but also
  //clear the stack of objects inside them. As a result, the vectors v2, v3 is
  //destroyed at the end of the scope. Most of the time, you don't want to use
  //the "new" operator because it creates leaks. However you MUST allocate
  //your objects and use pointers when using polymorphism.
  {
    //when you create object on the stack, the constructor is called. when you
    //use no arguments, like here, it will try to find a default constructor
    std::vector<int> v2;
    //But you can put your parameters after the variable name.
    //since vectors can be constructed from arrays, I can create an array
    // with {} (also works in C) and pass it parameter
    std::vector<int> v3({1, 2, 3});
    
    //ref are nothing as itself, they must reference something
    std::vector<int>& v2ref = v2;
    //modifying v2ref will modify v2 as well
    v2ref.push_back(1);
    //foreach in c++ uses the for keyword with this syntax 
    //for (#types name : variable) as in foreach (#types name in variable)
    //if you can, always use refs (const or not) since not using refs will
    //call the copy constructor each time and it's pretty slow
    for ( const auto& n : v2)
    {
      std::cout << n << " ";
    }
    std::cout << std::endl;

    //you cannot reaffect, here you're saying v2ref = v3, so v2 = v3
    v2ref = v3;
    for ( const auto& n : v2)
    {
      std::cout << n << " ";
    }
    std::cout << std::endl;
  }
  //Both v2 and v3 are now safely destroyed. When objects are destroyed, their
  //Destructor are called. Destructors have no arguments and are public.
  delete v1;
  return 0;
}
