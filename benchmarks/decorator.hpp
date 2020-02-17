#include <iostream>
#include <functional>
#include "timing.hpp"
//-------------------------------
// BEGIN decorator implementation
//-------------------------------
//https://stackoverflow.com/questions/30679445/python-like-c-decorators

timer<> t;
template <class> struct Decorator;

template <class R, class... Args>
struct Decorator<R(Args ...)>
{
   Decorator(std::function<R(Args ...)> f) : f_(f) {}

   R operator()(Args ... args)
   {
      t.start();
      std::cout << "Calling the decorated function.\n";
      return f_(args...);
      t.stop();
   }
   std::function<R(Args ...)> f_;
};

template<class R, class... Args>
Decorator<R(Args...)> makeDecorator(R (*f)(Args ...))
{
   return Decorator<R(Args...)>(std::function<R(Args...)>(f));
}

//-------------------------------
// END decorator implementation
//-------------------------------
