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
    auto test_6 = new Node<int>(60);

    test_1->set_rc(test_2);
    test_3->set_children(test_1, test_5);
    test_6->set_lc(test_3);
    test_5->set_lc(test_4);

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

    return 0;
}
