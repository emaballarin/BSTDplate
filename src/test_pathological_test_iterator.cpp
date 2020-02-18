#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto test_1 = new Node<int>(10);
    auto test_2 = new Node<int>(20);
    auto test_3 = new Node<int>(30);
    auto test_4 = new Node<int>(40);
    auto test_5 = new Node<int>(50);

    test_5->set_lc(test_1);
    test_1->set_rc(test_2);
    test_2->set_rc(test_3);
    test_3->set_rc(test_4);

    ////////////////////////////////////////////////////////////////////////////

    auto pippo = tree_iterator<Node<int>, false>{test_1};
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;

    (void)pippo;

    delete test_5;

    return 0;
}
