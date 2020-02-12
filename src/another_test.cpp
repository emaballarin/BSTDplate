// A simple test file

#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>

int main()
{
    auto pippo = new int(90);
    auto plutonode_1_pr = new Node<int>();
    auto plutonode_1_my = new Node<int>();
    auto plutonode_1_rc = new Node<int>();
    auto plutonode_1_lc = new Node<int>();

    plutonode_1_lc->write_elem(500);
    plutonode_1_my->write_elem(600);
    plutonode_1_pr->write_elem(700);
    plutonode_1_rc->write_elem(800);

    plutonode_1_my->set_both_children(plutonode_1_lc, plutonode_1_rc);

    plutonode_1_pr->set_lc(plutonode_1_my);

    auto test_iter = tree_iterator<Node<int>, false>(plutonode_1_lc);

    auto test_pluto = ++test_iter;
    test_pluto = ++test_pluto;
    //test_pluto->info();

    delete pippo;
    delete plutonode_1_pr;


    // Default return
    return 0;
}
