#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>


int main()
{
    auto tree = new bst<int, int>();
    auto tree_CP = new bst<int, int>();

    //auto pair_3 = tree->emplace(std::pair<int, int>({3, 30}));

    //std::cout << (tree->end());


    // LEAKS, as end() / cend() may be not implemented as of now
    //tree->find(1);
    std::cout << "START FIND:" << std::endl;

    //tree->end();
    //std::cout << "END FIND" << std::endl;
    //std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;

    auto pair_6 = tree->emplace(std::pair<int, int>({6, 60}));
    auto pair_6_bis = tree->emplace(std::pair<int, int>({6, 6000}));


    auto pair_3 = tree->emplace(std::pair<int, int>({3, 30}));
    auto pair_3_bis = tree->emplace(std::pair<int, int>({3, 3000}));


    auto pair_1 = tree->emplace(std::pair<int, int>({1, 10}));
    auto pair_1_bis = tree->emplace(std::pair<int, int>({1, 1000}));


    auto pair_2 = tree->emplace(std::pair<int, int>({2, 20}));
    auto pair_2_bis = tree->emplace(std::pair<int, int>({2, 2000}));


    auto pair_5 = tree->emplace(std::pair<int, int>({5, 50}));
    auto pair_5_bis = tree->emplace(std::pair<int, int>({5, 5000}));


    auto pair_4 = tree->emplace(std::pair<int, int>({4, 40}));
    auto pair_4_bis = tree->emplace(std::pair<int, int>({4, 4000}));


    std::cout << pair_6.first->read_elem().first << " " << pair_6.first->read_elem().second << " " << pair_6.second
              << std::endl;
    std::cout << pair_3.first->read_elem().first << " " << pair_3.first->read_elem().second << " " << pair_3.second
              << std::endl;
    std::cout << pair_1.first->read_elem().first << " " << pair_1.first->read_elem().second << " " << pair_1.second
              << std::endl;
    std::cout << pair_2.first->read_elem().first << " " << pair_2.first->read_elem().second << " " << pair_2.second
              << std::endl;
    std::cout << pair_5.first->read_elem().first << " " << pair_5.first->read_elem().second << " " << pair_5.second
              << std::endl;
    std::cout << pair_4.first->read_elem().first << " " << pair_4.first->read_elem().second << " " << pair_4.second
              << std::endl;

    std::cout << pair_6_bis.first->read_elem().first << " " << pair_6_bis.first->read_elem().second << " "
              << pair_6_bis.second << std::endl;
    std::cout << pair_3_bis.first->read_elem().first << " " << pair_3_bis.first->read_elem().second << " "
              << pair_3_bis.second << std::endl;
    std::cout << pair_1_bis.first->read_elem().first << " " << pair_1_bis.first->read_elem().second << " "
              << pair_1_bis.second << std::endl;
    std::cout << pair_2_bis.first->read_elem().first << " " << pair_2_bis.first->read_elem().second << " "
              << pair_2_bis.second << std::endl;
    std::cout << pair_5_bis.first->read_elem().first << " " << pair_5_bis.first->read_elem().second << " "
              << pair_5_bis.second << std::endl;
    std::cout << pair_4_bis.first->read_elem().first << " " << pair_4_bis.first->read_elem().second << " "
              << pair_4_bis.second << std::endl;

    std::cout << "FIND TEST:" << std::endl;
    //auto pippopluto = tree->find(1);
    auto pippopluto = tree->find(1);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    //
    pippopluto = tree->find(2);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    pippopluto = tree->find(3);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    pippopluto = tree->find(4);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    pippopluto = tree->find(5);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    pippopluto = tree->find(6);
    std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    //
    //pippopluto = tree->find(10);
    //std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    //pippopluto = tree->find(60);
    //std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;
    //pippopluto = tree->find(600);
    //std::cout << pippopluto->read_elem().first << " " << pippopluto->read_elem().second << std::endl;

    std::cout << tree->operator[](3) << std::endl;
    (*tree)[3] = 2;
    std::cout << (*tree)[3] << std::endl;

    *tree_CP = *tree;

    std::cout << *tree << std::endl;


    return 0;
}
