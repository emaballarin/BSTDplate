#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

template<typename T>
void debug(T);

int main()
{
    auto test_1 = new Node<int>(10);
    auto test_2 = new Node<int>(20);
    auto test_3 = new Node<int>(30);
    auto test_4 = new Node<int>(40);
    auto test_5 = new Node<int>(50);
    auto test_6 = new Node<int>(60);
    auto test_7 = new Node<int>(70);

    test_1->set_rc(test_2);
    test_3->set_children(test_1, test_5);
    test_6->set_lc(test_3);
    test_5->set_lc(test_4);
    test_6->set_rc(test_7);

    ////////////////////////////////////////////////////////////////////////////

    //conversion tests from const iter to iter and viceversa
    auto pippo = tree_iterator<Node<int>, false>{test_1};
    tree_iterator<Node<int>, false> pippo_copy{pippo};
    tree_iterator<Node<int>, true> pippo_const{pippo};
    tree_iterator<Node<int>, false> pippo_const_cast{pippo_const};
    auto pippo_move = new tree_iterator<Node<int>, false>{test_1};  //default move constr
    pippo_copy = *pippo_move;                                       //default move assign

    pippo_const = pippo_copy;

    //test ==
    auto end_pippo = tree_iterator<Node<int>, false>{test_7};
    auto end_pippo_const = tree_iterator<Node<int>, true>{test_7};


    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;
    ++pippo;
    std::cout << pippo->read_elem() << std::endl;

    std::cout << "Should be true: " << (pippo == end_pippo) << '\n'
              << "Should be true: " << (pippo == end_pippo_const) << '\n';

    delete test_6;
    delete pippo_move;

    return 0;
}
