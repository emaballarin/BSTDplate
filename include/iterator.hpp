/******************************************************************************\
 * |> ITERATOR a.k.a. iterator.hpp <|                                         *
 *                                                                            *
 * "Knowing too much is a great advantage and a greater curse"                *
 *                                                                            *
 *                          (on the choice of not relying explicitly on keys) *
 *                                                                            *
\******************************************************************************/

#pragma once

// SET NOEXCEPT WHERE NEEDED!

#include "node.hpp"

#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>

template<typename node,
         bool Const = true>  // Source: https://stackoverflow.com/questions/2150192
class tree_iterator
{
    public:
    using value_type = typename std::conditional<Const, const node, node>::type;
    using reference = typename std::conditional<Const, const node&, node&>::type;
    using pointer = typename std::conditional<Const, const node*, node*>::type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;  // Never used, but needed ;)

    // Custom: ctor | Default: cpctor, mvctor, cpasst, mvasst, dtor
    inline explicit tree_iterator(node*&) noexcept;

    inline reference operator*() const;
    inline pointer operator->() const;

    inline tree_iterator& operator++();
    inline tree_iterator operator++(int);  // Acceptable warning (clang-tidy)

    inline bool operator==(const tree_iterator&) const;
    inline bool operator!=(const tree_iterator&) const;


    private:
    value_type* current;

    inline tree_iterator& leftmost(tree_iterator&);
};

template<typename node, bool Const>
inline tree_iterator<node, Const>::tree_iterator(node*& given) noexcept : current{given} {};


template<typename node, bool Const>
inline typename tree_iterator<node, Const>::reference tree_iterator<node, Const>::operator*() const
{
    return *current;
}

template<typename node, bool Const>
inline typename tree_iterator<node, Const>::pointer tree_iterator<node, Const>::operator->() const
{
    return &(*(*this));
}

template<typename node, bool Const>
inline tree_iterator<node, Const>& tree_iterator<node, Const>::operator++()
{

    tree_iterator<node, Const> next{current};

    pointer& ptr_r = next->read_rc().get();  // Originally not reference. Why?

    if (ptr_r)
    {
        next.current = ptr_r;
        next = leftmost(next);
    }
    else if (!(next->read_lc().get()) && next->is_left())
    {
        next.current = next->read_pr().get();
    }
    else
    {
        while (!(next->is_left()) && !(next->read_pr()->read_rc()) && next->read_pr().get())
        {
            next.current = next->read_pr().get();
        }
        next.current =
          next ? next->read_pr().get() : current++;  // Avoid returning a nullptr (parent of root) instead of tree end
    }

    // Allow (N)RVO, however the compiler wants to perform it (Does it? Cool!)
    return next;
}

template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::operator++(int)  // Acceptable warning (clang-tidy)
{
    tree_iterator<node, Const> old{current};  // Always possible if called from in-range
    ++this;                                   // May result in UB (but not our problem; crf.: N.M. Josuttis, 1999)
    return old;
}

template<typename node, bool Const>
inline bool tree_iterator<node, Const>::operator==(const tree_iterator<node, Const>& given) const
{
    return &*current == &*(given->current);  // Since nodes are uncopyable, it should just satisfy identity
}

template<typename node, bool Const>
inline bool tree_iterator<node, Const>::operator!=(const tree_iterator<node, Const>& given) const
{
    return &*current != &*(given->current);  // Since nodes are uncopyable, it should just satisfy non-identity
}

template<typename node, bool Const>
inline tree_iterator<node, Const>& tree_iterator<node, Const>::leftmost(tree_iterator<node, Const>& given)
{
    tree_iterator<node, Const> next{given};

    while (next->read_lc())
    {
        next.current = next->read_lc.get();
    }
    return next;
}
