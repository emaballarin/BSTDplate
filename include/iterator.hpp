#pragma once
#include <memory>
#include <iterator>
#include "bst.hpp"
#include "node.hpp"
#include <iostream>

template<typename node>
class tree_iterator{
    public:
        using value_type = typename node::value_type; // we use typename to avoid the compiler to think of it as the name of a static variable of node
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::forward_iterator_tag;

        reference operator*() const;
        pointer operator->() const;

        tree_iterator& operator++();
        tree_iterator operator++(int);
        bool operator==(const tree_iterator& rhs) const;
        bool operator!=(const tree_iterator& rhs) const;



    private:
        Node<value_type>* current;


};

template<typename node>
class const_tree_iterator{
    public:
        using value_type = typename node::value_type; // we use typename to avoid the compiler to think of it as the name of a static variable of node
        using const_reference = const value_type&;
        using const_pointer = const value_type*;
        using iterator_category = std::forward_iterator_tag;

        const_reference operator*() const;
        const_pointer operator->() const;

        const_tree_iterator& operator++();
        const_tree_iterator operator++(int);
        bool operator==(const const_tree_iterator& rhs) const;
        bool operator!=(const const_tree_iterator& rhs) const;



    private:
        const Node<value_type>* current;


};
