#include <iostream>
#include <functional>
#include "timing.hpp"
#include "random.hpp"
#include <bst.hpp>
#include <node.hpp>
#include <iterator.hpp>
#include <fstream>
#include <random>
#include <utility>
#include <map>
#include <unordered_map>
#include <string>

template<typename T>
struct Hello{
  T num;
  Hello(T i): num{i}{}
  bool hellow(T i){return num==i;}
};

timer<> t;
template<class Tree, typename Pair>
void benchmarks(Rand_int r, std::string title, std::size_t max){

  std::ofstream file;
  file.open("./results/"+title+".txt");

  for(std::size_t j{1}; j<max ; ++j){
    auto tree = new Tree();

    for(std::size_t i{0}; i<j ; ++i){
      int temp(r());
      tree->insert(Pair{temp, temp});
    }

    int temp{r()};
    t.start();
    tree->find(temp);
    t.stop(file);
  }
  file.close();
}

template<class Tree, typename Pair>
void benchmarks(Rand_double r, std::string title, std::size_t max){

  std::ofstream file;
  file.open("./results/"+title+".txt");

  for(std::size_t j{1}; j<max ; ++j){
    auto tree = new Tree();

    for(std::size_t i{0}; i<j ; ++i){
      double temp(r());
      tree->insert(Pair{temp, temp});
    }

    double temp{r()};
    tree->insert(Pair{temp, temp});

    t.start();
    tree->find(temp);
    t.stop(file);
  }
  file.close();
}


int main()
{

  Rand_int ri{1,10000, 0};
  Rand_double rd{0, 1, 0};
  std::size_t max{1000};

  benchmarks<bst<int, int>, std::pair<int, int>>(ri, "bst_int_int", max);
  benchmarks<std::map<int, int>, std::pair<int, int>>(ri, "map_int_int", max);
 //  benchmarks<std::unordered_map<int, int>, std::pair<int, int>>(ri, "umap_int_int", max);
 //
 //  benchmarks<bst<double, double>,std::pair<double, double>>(rd, "bst_double_double", max);
 //  benchmarks<std::map<double, double>,std::pair<double, double>>(rd, "map_double_double", max);
 //  benchmarks<std::unordered_map<double, double>,std::pair<double, double>>(rd, "umap_double_double", max);
 //
 //  benchmarks<bst<char, char>, std::pair<char, char>>(ri, "bst_char_char", max);
 //  benchmarks<std::map<char, char>, std::pair<char, char>>(ri, "map_char_char", max);
 //  benchmarks<std::unordered_map<char, char>,std::pair<char, char>>(ri, "umap_char_char", max);
}
