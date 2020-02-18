#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto tree = new bst<int, int>();

    auto pair_6 = tree->emplace(std::pair<int, int>({6, 60}));
    auto pair_6_bis = tree->emplace(std::pair<int, int>({6, 6000}));

    auto pair_5 = tree->emplace(std::pair<int, int>({5, 50}));
    auto pair_5_bis = tree->emplace(std::pair<int, int>({5, 5000}));

    auto pair_4 = tree->emplace(std::pair<int, int>({4, 40}));
    auto pair_4_bis = tree->emplace(std::pair<int, int>({4, 4000}));

    auto pair_3 = tree->emplace(std::pair<int, int>({3, 30}));
    auto pair_3_bis = tree->emplace(std::pair<int, int>({3, 3000}));

    auto pair_1 = tree->emplace(std::pair<int, int>({1, 10}));
    auto pair_1_bis = tree->emplace(std::pair<int, int>({1, 1000}));

    auto pair_2 = tree->emplace(std::pair<int, int>({2, 20}));
    auto pair_2_bis = tree->emplace(std::pair<int, int>({2, 2000}));

    tree->balance();
    std::cout << *tree;


    delete tree;
    return 0;
}
