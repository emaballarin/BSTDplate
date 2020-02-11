#pragma once
#include <iterator>
#include <memory>
//#include "bst.hpp"
#include "node.hpp"

#include <iostream>
#include <type_traits>

template<
  typename node,
  bool Const =
    true>  //https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
class tree_iterator
{
    public:
    using value_type = typename std::conditional<Const, const node, node>::type;
    using reference = typename std::conditional<Const, const node&, node&>::type;
    using pointer = typename std::conditional<Const, const node*, node*>::type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;  //not used

    //constructor. Default move and copy constructors/assignments
    explicit tree_iterator(node*&) noexcept;

    reference operator*() const;
    pointer operator->() const;

    tree_iterator& operator++();
    tree_iterator operator++(int);

    bool operator==(const tree_iterator&) const;
    bool operator!=(const tree_iterator&) const;


    private:
    value_type* current;

    tree_iterator& leftmost(tree_iterator&);
    bool next_exist(tree_iterator&) const;
};

template<typename node, bool Const>
tree_iterator<node, Const>::tree_iterator(node*& given) noexcept : current{given} {};


template<typename node, bool Const>
typename tree_iterator<node, Const>::reference tree_iterator<node, Const>::operator*() const
{
    return *current;
}

template<typename node, bool Const>
typename tree_iterator<node, Const>::pointer tree_iterator<node, Const>::operator->() const
{
    return &(*(*this));
}

template<typename node, bool Const>
tree_iterator<node, Const>& tree_iterator<node, Const>::operator++()
{

    tree_iterator<node, Const> next{current};

    pointer ptr_r = next->read_rc().get();
    if (ptr_r)
    {
        next.current = ptr_r;
        return left_most(next);
    }
    else if (!(next->read_lc().get()) && next->is_left())
    {
        next.current = next->read_pr().get();
        return next;
    }
    else
    {
        while (!(next->is_left()) && !(next->read_pr()->read_rc()) && next->read_pr.get())
        {
            next.current = next->read_pr().get();
        }
        next.current = next ? next->read_pr().get() : current++;
        return next;
    }
}

template<typename node, bool Const>
tree_iterator<node, Const> tree_iterator<node, Const>::operator++(int)
{
    if (next_exist())
    {
        tree_iterator<node, Const> old{current};
        ++this;
        return old;
    }
}

template<typename node, bool Const>
bool tree_iterator<node, Const>::operator==(const tree_iterator<node, Const>& given) const
{
    return &*current == &*(given->current);
}

template<typename node, bool Const>
bool tree_iterator<node, Const>::operator!=(const tree_iterator<node, Const>& given) const
{
    return &*current != &*(given->current);
}

template<typename node, bool Const>
tree_iterator<node, Const>& tree_iterator<node, Const>::leftmost(tree_iterator<node, Const>& given)
{
    tree_iterator<node, Const> next{current};

    while (next->read_lc())
    {
        next.current = next->read_lc.get();
    }

    return next;
}

template<typename node, bool Const>
bool tree_iterator<node, Const>::next_exist(tree_iterator& given) const
{
    while (!(given->is_left()) && !(given->read_pr()->read_rc()) && given->read_pr.get())
    {
        given.current = given->read_pr().get();
    }
}
