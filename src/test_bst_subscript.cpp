#include "bst.hpp"
#include "iterator.hpp"
#include "node.hpp"

int main()
{
    auto tree = new bst<int, int>();

    tree->insert(std::pair<int, int>({7, 70}));
    tree->insert(std::pair<int, int>({7, 70}));


    tree->insert(std::pair<int, int>({3, 30}));
    tree->insert(std::pair<int, int>({3, 30}));


    tree->insert(std::pair<int, int>({1, 10}));
    tree->insert(std::pair<int, int>({1, 10}));


    tree->insert(std::pair<int, int>({2, 20}));
    tree->insert(std::pair<int, int>({2, 20}));


    tree->insert(std::pair<int, int>({5, 50}));
    tree->insert(std::pair<int, int>({5, 50}));


    tree->insert(std::pair<int, int>({4, 40}));
    tree->insert(std::pair<int, int>({4, 40}));

    tree->insert(std::pair<int, int>({6, 60}));
    tree->insert(std::pair<int, int>({6, 60}));

    tree->insert(std::pair<int, int>({8, 80}));
    tree->insert(std::pair<int, int>({8, 80}));


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
    ++pippo;

    (*tree)[8] = 2;
    std::cout << (*tree)[8] << '\n';
    delete tree;
    return 0;
}
