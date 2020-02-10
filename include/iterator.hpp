#pragma once
#include <memory>
#include <iterator>
#include "bst.hpp"
#include "node.hpp"
#include <iostream>
#include <type_traits>

template<typename node, bool Const=true>//https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
class tree_iterator
{

    public:
    using value_type = typename node::value_type;
    using reference = typename std::conditional< Const, const node &, node & >::type;
    using pointer = typename std::conditional< Const, const node *, node * >::type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    reference operator*() const;
    pointer operator->() const;

    tree_iterator& operator++();
    tree_iterator operator++(int);
    bool operator==(const tree_iterator& rhs) const;
    bool operator!=(const tree_iterator& rhs) const;

    private:
    node* current;
};

template<typename node, bool Const>
typename tree_iterator<node, Const>::reference tree_iterator<node, Const>::operator*() const{}
