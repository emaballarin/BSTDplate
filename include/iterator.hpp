#include <memory>
#include <iterator>
#include "bst.hpp"
#include <iostream>

template<typename node>
class iterator{
    public:
        using value_type = typename node::value_type; // we use typename to avoid the compiler to think of it as the name of a static variable of node
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::forward_iterator_tag;

        iterator& operator++();
        iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;



    private:
        Node* current;

    
};
