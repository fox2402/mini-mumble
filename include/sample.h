#pragma once

//Sample class just to have something in the include folder therefore
//being able to push it.

//namespaces are like folder in your code. Here to access Sample class you'll
//type sample::Sample. "::" is used to search inside namespaces. Class are
//also namespaces to acces a static function you'll go for 
//sample::Sample::static_sample();
//Note that you don't need to specify the namespace when you are inside the
//namespace. Hence to call static_sample from inside the class you just write
//static_sample();

//Never implements in .h as in C (well in some case you might want to but
//let's not talk about that now)

namespace sample
{
  class Sample
  {
    public:
    //Constructor
      Sample();
    //Destructor
      ~Sample();
    //A static function
      static int static_sample();

    protected:
    //virtuals functions are written this way
      virtual int virtual_function();
    //abstract are much like virtuals
      virtual int abstract_function() = 0;
    private:
    //value, reference and pointer
    //reference works like in c#, if you modify the ref, the original is
    //modified as well. Use it whenever you can.
      int a;
      int& a;
      int* a;
  };
  //Derived class is written this way
  class Sample2 : Sample
  {

  }
}
