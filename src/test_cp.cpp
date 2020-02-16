#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

int main()
{
    auto tree_1 = new bst<int, int>();
    tree_1->emplace(std::pair<int, int>({3, 30}));
    tree_1->emplace(std::pair<int, int>({4, 40}));
    tree_1->emplace(std::pair<int, int>({2, 40}));
    tree_1->emplace(std::pair<int, int>({5, 40}));
    auto tree_2 = new bst<int, int>();


    std::cout << *tree_1 << std::endl;

    *tree_2 = std::move(*tree_1);

    delete tree_1;

    std::cout << *tree_2 << std::endl;


    delete tree_2;

    return 0;
}
