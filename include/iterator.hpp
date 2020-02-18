/******************************************************************************\
 * |> ITERATOR a.k.a. iterator.hpp <|                                         *
 *                                                                            *
 * "Knowing too much is a great advantage and a greater curse"                *
 *                                                                            *
 *                          (on the choice of not relying explicitly on keys) *
 *                                                                            *
\******************************************************************************/

/**
 * @file iterator.hpp
 * @author Leonardo Arrighi
 * @author Francesco Romor
 * @author Emanuele Ballarin
 * @date 18 February 2020
 * @brief Header file containing the implementation of an iterator iterating over a graph composed of type-templated binary nodes.
 */

#pragma once  // Easy ODR

#include "node.hpp"

#include <iostream>
#include <iterator>
#include <memory>
#include <type_traits>

/**
 * @brief Type-templated forward tree_iterator
 * @tparam node Typename of the node
 * @tparam Const Bool to represent a const iterator (true) or an iterator (false)
 */
template<typename node,
         bool Const = true>  // Cfr.: https://stackoverflow.com/questions/2150192
class tree_iterator
{
    public:
    using value_type = typename std::conditional<Const, const node, node>::type;
    using reference = typename std::conditional<Const, const node&, node&>::type;
    using pointer = typename std::conditional<Const, const node*, node*>::type;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;


    friend tree_iterator<node, !Const>;

    tree_iterator() noexcept = delete; //deleted since it would coincide with end
    /**
     * @brief custom constructor
     * @param given The pointer to the node
     */
    explicit tree_iterator(value_type* given) noexcept : current{given} {}

    /**
     * @brief constructor from const or non const iterator
     * @param given The iterator to be copied and eventually const casted
     */
    template<bool T>
    tree_iterator(tree_iterator<node, T> given) noexcept
    {
        this->current = const_cast<pointer>(given.current);
    }

    /**
     * @brief Dereference operator
     * @return reference to the current node
     */
    reference operator*() const{ return *current;}

    /**
     * @brief Access member of current
     * @return pointer to current node
     */
    pointer operator->() const{return &(*(*this));}

    tree_iterator operator++() noexcept;
    inline tree_iterator operator++(int) noexcept;  // Acceptable warning (clang-tidy)

    /**
     * @brief Equality operator
     * @tparam T Bool to compare also const interators with non-const iterators
     * @param given The iterator to be compared to
     * @return bool
     */
    template<bool T>
    bool operator==(const tree_iterator<node, T>& given) const noexcept
    {
      return this->current == given.current;
    }

    /**
     * @brief Inequality operator
     * @tparam T Bool to compare also const interators with non-const iterators
     * @param given The iterator to be compared to
     * @return bool
     */
    template<bool T>
    bool operator!=(const tree_iterator<node, T>& given) const noexcept
    {
      return !(this->current == given.current);
    }

    inline tree_iterator next() noexcept;

    private:
    /** Current node to be pointed to by the iterator*/
    value_type* current;

    inline tree_iterator<node, Const> leftmost(tree_iterator<node, Const>&) noexcept;
};

/**
 * @brief Pre-increment of forward iterator
 * @tparam node Typename of the node
 * @tparam Const Bool to represent a const iterator (true) or an iterator (false)
 *
 * Traverse the tree in order to find the next node to point to.
 */
template<typename node, bool Const>
tree_iterator<node, Const> tree_iterator<node, Const>::operator++() noexcept
{
    tree_iterator<node, Const> next{current};
    if (pointer ptr_r = next->read_rc().get(); ptr_r)
    {
        next.current = ptr_r;
        next = leftmost(next);
    }
    else if (!(next->read_lc().get()) && next->is_left() && (next->read_pr().get() == nullptr))
    {
        next.current = next->read_pr().get();
    }
    else
    {
        while (next->is_right())
        {
            next.current = next->read_pr().get();
        }
        next.current = (next->read_pr().get() != nullptr)
                         ? next->read_pr().get()
                         :nullptr;
    }

    // Allow (N)RVO
    *this = next;  // Allow optimization by pipelining
    return next;
}

/**
 * @brief Post-increment of forward iterator
 * @tparam node Typename of the node
 * @tparam Const Bool to represent a const iterator (true) or an iterator (false)
 * @return this before being incremented
 */
template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::operator++(int) noexcept  // Acceptable warning (clang-tidy)
{
    tree_iterator<node, Const> old{current};  // Always possible if called from in-range
    ++*this;                                  // May result in UB (but not our problem; crf.: N.M. Josuttis, 1999)
    return old;
}

/**
 * @brief return the iterator associated to the leaf obtained going leftmost from given
 * @tparam node Typename of the node
 * @tparam Const Bool to represent a const iterator (true) or an iterator (false)
 * @param given the starting node
 * @return iterator to the leftmost leaf obtained from the given node
 */
template<typename node, bool Const>
tree_iterator<node, Const> tree_iterator<node, Const>::leftmost(tree_iterator<node, Const>& given) noexcept
{
    tree_iterator<node, Const> next{given.current};
    while (next->read_lc())
    {
        next.current = next->read_lc().get();
    }
    return next;
}

/**
 * @brief returns the iterator pointing to the next node
 * @tparam node Typename of the node
 * @tparam Const Bool to represent a const iterator (true) or an iterator (false)
 * @return next iterator
 */
template<typename node, bool Const>
inline tree_iterator<node, Const> tree_iterator<node, Const>::next() noexcept
{
    tree_iterator<node, Const> this_copy{*this};
    return tree_iterator<node, Const>{++this_copy};
}
