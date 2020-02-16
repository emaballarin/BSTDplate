// A simple test file

#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

template<typename T>
void debug(T);

int main()
{
    tree_iterator<Node<int>, false> iter{new Node<int>{2}};
    std::experimental::observer_ptr<int> ptr;


    std::cout << "Hello, world!" << std::endl << ptr.get() << iter->read_elem() << '\n';
    ++iter;
    //debug(*iter);

    // std::cout << "Hello, world!" << std::endl
    //           << *iter << '\n';
    // std::cout << "Hello, world!" << std::endl
    //           << *(++iter) << '\n';
    return 0;
}
