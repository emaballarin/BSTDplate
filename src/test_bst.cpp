#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto tree = new bst<int, int>();

    tree->insert(std::pair<int, int>({1, 10}));
    tree->insert(std::pair<int, int>({2, 20}));
    tree->insert(std::pair<int, int>({3, 30}));
    //tree->insert(std::pair<int, int>({4, 40}));
    //tree->insert(std::pair<int, int>({5, 50}));
    //tree->insert(std::pair<int, int>({6, 60}));

    delete tree;
    return 0;
}
