#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <cassert>
#include <experimental/memory>
#include <type_traits>
#include <utility>

using Node_t = Node<int>;

int main()
{
    auto test_1 = new Node_t(10);
    auto test_2 = new Node_t(20);
    auto test_3 = new Node_t(30);
    auto test_4 = new Node_t(40);
    auto test_5 = new Node_t(50);
    auto test_6 = new Node_t(60);

    test_1->set_rc(test_2);
    test_3->set_children(test_1, test_5);
    test_6->set_lc(test_3);
    test_5->set_lc(test_4);

    std::cout << "~~ ADDRESSES: ~~" << std::endl;
    std::cout << test_1 << std::endl;
    std::cout << test_2 << std::endl;
    std::cout << test_3 << std::endl;
    std::cout << test_4 << std::endl;
    std::cout << test_5 << std::endl;
    std::cout << test_6 << std::endl;

    delete test_6;

    return 0;
}
