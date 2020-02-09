#include <iostream>
#include <iterator>
#include "bst.hpp"
#include "node.hpp"
#include <memory>

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

template<typename node>
iterator<node>& iterator<node>::operator++(){
    //++current;
    //return * this;
}

template<typename node>
iterator<node> iterator<node>::operator++(int){
    //iterator tmp(*this);
    //operator++();
    //return tmp;
}

template<typename node>
iterator<node>::reference iterator<node>::operator*() const{
    return * current;

}

template<typename node>
iterator<node>::pointer iterator<node>::operator->() const{
    return current;
}

template<typename node>
bool iterator<node>::operator==(const iterator& rhs) const{
    return current==rhs.current;
}

template<typename node>
bool iterator<node>::operator!=(const iterator& rhs) const{
    return current!=rhs.current;
}