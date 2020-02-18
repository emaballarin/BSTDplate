// A simple test file

#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

template<typename T>
void debug(T);

int main()
{
    auto treetest = new bst<int, int>();

    treetest->kv_insert(std::move(50), std::move(1223));

    std::cout << treetest->find(50)->read_elem().second << std::endl;

    return 0;
}
