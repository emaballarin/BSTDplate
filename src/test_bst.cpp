#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto tree = new bst<int, int>();

    auto pair_6 = tree->insert(std::pair<int, int>({6, 60}));
    auto pair_6_bis = tree->insert(std::pair<int, int>({6, 60}));


    auto pair_3 = tree->insert(std::pair<int, int>({3, 30}));
    auto pair_3_bis = tree->insert(std::pair<int, int>({3, 30}));


    auto pair_1 = tree->insert(std::pair<int, int>({1, 10}));
    auto pair_1_bis = tree->insert(std::pair<int, int>({1, 10}));


    auto pair_2 = tree->insert(std::pair<int, int>({2, 20}));
    auto pair_2_bis = tree->insert(std::pair<int, int>({2, 20}));


    auto pair_5 = tree->insert(std::pair<int, int>({5, 50}));
    auto pair_5_bis = tree->insert(std::pair<int, int>({5, 50}));


    auto pair_4 = tree->insert(std::pair<int, int>({4, 40}));
    auto pair_4_bis = tree->insert(std::pair<int, int>({4, 40}));

    auto pair_7 = tree->insert(std::pair<int, int>({7, 70}));
    auto pair_7_bis = tree->insert(std::pair<int, int>({7, 70}));


    //test insert
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

    auto pippo = tree->cbegin();
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    ++pippo;
    std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;
    // ++pippo;
    // std::cout << "(" << pippo->read_elem().first << ":" << pippo->read_elem().second << ")" << std::endl;

    std::cout << *tree << std::endl;


    //delete tree;
    return 0;
}
