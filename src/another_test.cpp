// A simple test file

#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

template<class... Types>
void pippoprova(Types&&... args)
{
    ((std::cout << args), ...) << std::endl;
}

int main()
{
    // Default return
    pippoprova(1, 2, "hello", "ano");
    return 0;
}
