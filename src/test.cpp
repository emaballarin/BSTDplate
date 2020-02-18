#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

bool pippocmp(int pippo, int pluto)
{

    return pippo < pluto;
}


int main()
{

    auto plutocmp = &pippocmp;


    auto treetest = new bst<int, int, decltype(pippocmp)>(pippocmp);

    treetest->emplace(std::move(50), std::move(1223));
    treetest->emplace(std::move(565), std::move(99999));
    treetest->emplace(std::move(5440), std::move(1223));

    std::cout << treetest->find(565)->read_elem().second << std::endl;

    delete treetest;

    return 0;
}
