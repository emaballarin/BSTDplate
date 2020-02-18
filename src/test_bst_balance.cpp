#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto tree = new bst<int, int>();

    tree->emplace(std::pair<int, int>({6, 60}));
    tree->emplace(std::pair<int, int>({6, 6000}));

    tree->emplace(std::pair<int, int>({5, 50}));
    tree->emplace(std::pair<int, int>({5, 5000}));

    tree->emplace(std::pair<int, int>({4, 40}));
    tree->emplace(std::pair<int, int>({4, 4000}));

    tree->emplace(std::pair<int, int>({3, 30}));
    tree->emplace(std::pair<int, int>({3, 3000}));

    tree->emplace(std::pair<int, int>({1, 10}));
    tree->emplace(std::pair<int, int>({1, 1000}));

    tree->emplace(std::pair<int, int>({2, 20}));
    tree->emplace(std::pair<int, int>({2, 2000}));

    tree->balance();
    std::cout << *tree << std::endl;


    delete tree;
    return 0;
}
