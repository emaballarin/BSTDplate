#include "random.hpp"
#include "timing.hpp"

#include <bst.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator.hpp>
#include <map>
#include <node.hpp>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>


timer<> t;
template<class Tree, typename Pair, bool bst_custom = false>
void benchmarks(Rand_int r, std::string title, std::size_t max)
{

    std::ofstream file;
    file.open("./results/" + title + ".txt");

    //    for (std::size_t j{1}; j < max; ++j)
    for (std::size_t j{1}; j < max; j += 10000)
    {
        auto tree = new Tree();

        for (std::size_t i{0}; i < j; ++i)
        {
            int temp(r());
            tree->insert(Pair{temp, temp});
        }

        long unsigned int temp{r()};
        t.start();
        tree->find(temp);
        t.stop(file);
        delete tree;
    }
    file.close();
}

template<class Tree, typename Pair, bool bst_custom = false>
void benchmarks(Rand_double r, std::string title, std::size_t max)
{

    std::ofstream file;
    file.open("./results/" + title + ".txt");

    //    for (std::size_t j{1}; j < max; ++j)
    for (std::size_t j{1}; j < max; j += 10000)
    {
        auto tree = new Tree();

        for (std::size_t i{0}; i < j; ++i)
        {
            double temp(r());
            tree->insert(Pair{temp, temp});
        }

        double temp{r()};
        tree->insert(Pair{temp, temp});

        t.start();
        tree->find(temp);
        t.stop(file);
        delete tree;
    }
    file.close();
}

int main()
{
    constexpr std::size_t max{10000000};

    Rand_int ri{1, 1000000, 0};
    Rand_double rd{0, 1, 0};
    Rand_int rc{97, 122, 0};

    //auto tree = new bst<int, int>();
    // for (std::size_t j{1}; j < max; j += 10000)
    // {
    //   auto tree = new bst<int, int>();
    //   for (std::size_t i{0}; i <j; ++i)
    //   {
    //       int temp(ri());
    //       tree->insert(std::pair<int, int>{temp, temp});
    //   }
    //
    //   int temp{ri()};
    //   tree->insert(std::pair<int, int>{temp, temp});
    //   tree->balance();
    //   //std::cout << *tree;
    //   t.start();
    //   tree->find(temp);
    //   t.stop();
    //   delete tree;
    // }

    benchmarks<bst<int, int>, std::pair<int, int>, true>(ri, "bst_int_int", max);
    benchmarks<std::map<int, int>, std::pair<int, int>>(ri, "map_int_int", max);
    benchmarks<std::unordered_map<int, int>, std::pair<int, int>>(ri, "umap_int_int", max);

    benchmarks<bst<double, double>, std::pair<double, double>, true>(rd, "bst_double_double", max);
    benchmarks<std::map<double, double>, std::pair<double, double>>(rd, "map_double_double", max);
    benchmarks<std::unordered_map<double, double>, std::pair<double, double>>(rd, "umap_double_double", max);

    benchmarks<bst<char, char>, std::pair<char, char>, true>(rc, "bst_char_char", max);
    benchmarks<std::map<char, char>, std::pair<char, char>>(rc, "map_char_char", max);
    benchmarks<std::unordered_map<char, char>, std::pair<char, char>>(rc, "umap_char_char", max);
}
