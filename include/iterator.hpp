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


    friend tree_iterator<node, !Const>;

    // Custom: ctor | Default: cpctor, mvctor, cpasst, mvasst, dtor
    tree_iterator() = default;
    inline explicit tree_iterator(value_type*) noexcept;

    template<bool T>
    tree_iterator(tree_iterator<node, T> given){
      this->current = const_cast<pointer>(given.current);
    }

    // explicit tree_iterator(tree_iterator<node, false> given){
    //   this->current = given.current;
    // }

    inline reference operator*() const;
    inline pointer operator->() const;

    inline tree_iterator operator++();
    inline tree_iterator operator++(int);  // Acceptable warning (clang-tidy)

    template<bool T>
    inline bool operator==(const tree_iterator<node, T>&) const;
    template<bool T>
    inline bool operator!=(const tree_iterator<node, T>&) const;

    inline tree_iterator next();

    inline tree_iterator<node, true> constify(tree_iterator<node, false>);
    inline tree_iterator<node, false> unconstify(tree_iterator<node, true>);

    tree_iterator<node, true> constify(){
       tree_iterator<node,true> iter{this.current};
      return iter;
    }
    private:

    value_type* current;

    inline tree_iterator<node, Const> leftmost(tree_iterator<node, Const>&) noexcept;
};

template<typename node, bool Const>
//inline tree_iterator<node, Const>::tree_iterator(node* given) noexcept : current{given} {};
inline tree_iterator<node, Const>::tree_iterator(value_type* given) noexcept : current{given} {};


template<typename node, bool Const>
inline typename tree_iterator<node, Const>::reference tree_iterator<node, Const>::operator*() const
{
    return *current;
}

template<typename node, bool Const>
inline typename tree_iterator<node, Const>::pointer tree_iterator<node, Const>::operator->() const
{
    return &(*(*this));
    //return &(current);
}

template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::operator++()
{
    tree_iterator<node, Const> next{current};
    //tree_iterator<node, false> next{const_cast<value_type*>(current)};
    //std::cout << "BEGIN";
    if (pointer ptr_r = next->read_rc().get(); ptr_r)
    {
        next.current = ptr_r;
        next = leftmost(next);
    }
    else if (!(next->read_lc().get()) && next->is_left() && (next->read_pr().get()==nullptr))
    {
        next.current = next->read_pr().get();
    }
    else
    {
        while (next->is_right())
        {
            next.current = next->read_pr().get();
        }
        next.current = (next->read_pr().get()!=nullptr) ? next->read_pr().get(): ++current;  // Avoid returning a nullptr (parent of root) instead of tree end
    }

    // Allow (N)RVO, however the compiler wants to perform it (Does it? Cool!)
    *this = next;  // Pipelining
    return next;
}

template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::operator++(int)  // Acceptable warning (clang-tidy)
{
    tree_iterator<node, Const> old{current};  // Always possible if called from in-range
    ++*this;                                  // May result in UB (but not our problem; crf.: N.M. Josuttis, 1999)
    return old;
}

template<typename node, bool Const>
template<bool T>
inline bool tree_iterator<node, Const>::operator==(const tree_iterator<node, T>& given) const
{
    return this->current == given.current;  // Since nodes are uncopyable, it should just satisfy identity
}

template<typename node, bool Const>
template<bool T>
inline bool tree_iterator<node, Const>::operator!=(const tree_iterator<node, T>& given) const
{
    return !(this->current == given.current);  // Since nodes are uncopyable, it should just satisfy non-identity
}

template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::leftmost(tree_iterator<node, Const>& given) noexcept
{
    tree_iterator<node, Const> next{given.current};
    while (next->read_lc())
    {
        next.current = next->read_lc().get();
    }
    return next;
}

template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::next()
{
    return tree_iterator<node, Const>{(++std::copy(*this)).current};  // Will it work ??
}

// template<typename node, bool Const>
// inline tree_iterator<node, true> tree_iterator<node, Const>::constify(tree_iterator<node, false> given)
// {
//     return tree_iterator<node, true>{given.current};
// }
//
// template<typename node, bool Const>
// inline tree_iterator<node, false> tree_iterator<node, Const>::unconstify(tree_iterator<node, true> given)
// {
//     return tree_iterator<node, false>{given.current};
// }
