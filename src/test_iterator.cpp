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
    auto test_8 = new Node<int>(80);

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
    tree_iterator<Node<int>, false> pippo_assign = pippo_copy;
    tree_iterator<Node<int>, false> pippo_assign_c = pippo_const;//it works default copy assign
    auto pippo_move = new tree_iterator<Node<int>, false>{test_1};//default move constr
    pippo_copy = *pippo_move;//default move assign
    //debug(pippo_copy);

    //OK:error invalid conversion from ‘tree_iterator<Node<int>, true>::value_type*’ {aka ‘const Node<int>*’} to ‘tree_iterator<Node<int>, false>::value_type*’ {aka ‘Node<int>*’}
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

    std::cout << "Should be true: " <<(pippo==end_pippo) << '\n'
              << "Should be true: " <<(pippo==end_pippo_const) << '\n';

    //static_cast<void>(iter_const);
    //static_cast<void>(end_pippo);
    //static_cast<void>(end_pippo_const);
    //    std::cout << "~~ ADDRESSES: ~~" << std::endl;
    //    std::cout << test_1 << std::endl;
    //    std::cout << test_2 << std::endl;
    //    std::cout << test_3 << std::endl;
    //    std::cout << test_4 << std::endl;
    //    std::cout << test_5 << std::endl;
    //    std::cout << test_6 << std::endl;
    //
    //    std::cout << "~~ NODES: ~~" << std::endl;
    //    test_1->info();
    //    test_2->info();
    //    test_3->info();
    //    test_4->info();
    //    test_5->info();
    //    test_6->info();

    delete test_6;

    return 0;
}
