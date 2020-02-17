#include <random>
#include <functional>
#pragma once

//from C++ the programming language Bjarne Stroustrup
class Rand_int{
public:
  Rand_int(int lo, int hi, int _seed){
    re.seed(_seed);
    dist = std::uniform_int_distribution<>(lo, hi);
  }
  int operator()(){return dist(re);}
  std::default_random_engine re;
  std::uniform_int_distribution<> dist;
};

class Rand_double{
public:
  Rand_double(double lo, double hi, int _seed):
  r(std::bind(std::uniform_real_distribution<>(lo, hi), re)),
  re{std::default_random_engine(_seed)}{}
    //re.seed(_seed);
    //r =std::function<double()>(std::bind(std::uniform_real_distribution<>(lo, hi), re()));
  //}
  double operator()(){return r();}
  std::function<double()> r;
  std::default_random_engine re;
};
