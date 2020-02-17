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
    auto test_7 = new Node_t();

    test_1->set_rc(test_2);
    test_3->set_children(test_1, test_5);
    test_6->set_lc(test_3);
    test_5->set_lc(test_4);
    test_1->set_rc(test_7);

    std::cout << "~~ ADDRESSES: ~~" << std::endl;
    std::cout << test_1 << std::endl;
    std::cout << test_2 << std::endl;
    std::cout << test_3 << std::endl;
    std::cout << test_4 << std::endl;
    std::cout << test_5 << std::endl;
    std::cout << test_6 << std::endl;

    std::cout << "~~ NODES: ~~" << std::endl;
    // test_1->info();
    // test_2->info();
    // test_3->info();
    // test_4->info();
    // test_5->info();
    // test_6->info();

    return 0;
}

void default_constructor()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void constructor()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void move()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void copy()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void read()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void is_children()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void set()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void detach()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}

void null()
{
    Node_t node;
    std::unique_ptr<Node_t> unique;
    std::experimental::observer_ptr<Node_t> obs;
    assert(node.read_rc() == unique);
    assert(node.read_lc() == unique);
    assert(node.read_pr() == obs);
    std::cout << "pass deafault constructor" << '\n';
}
