#include "node.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main()
{
    std::unique_ptr<Node<int>> pippo1 = std::make_unique<Node<int>>();
    std::unique_ptr<Node<int>> pippo2 = std::make_unique<Node<int>>();
    std::unique_ptr<Node<int>> pippo3 = std::make_unique<Node<int>>();
    std::unique_ptr<Node<int>> paperino = std::make_unique<Node<int>>(nullptr, nullptr, nullptr);
    return 0;
}
