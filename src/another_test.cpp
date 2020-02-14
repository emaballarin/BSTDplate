// A simple test file

#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <functional>
#include <iostream>

int main()
{
    auto pippo = new bst<int, int, std::less<int>>;
    std::pair<int, int> ktest = std::pair<int, int>{10, 200};


    auto getpair = pippo->insert(ktest);

    std::cout << getpair.first->read_elem().first << getpair.first->read_elem().second << getpair.second << std::endl;


    (void)pippo;

    delete pippo;

    return 0;
}
