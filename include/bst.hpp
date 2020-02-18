/******************************************************************************\
 * |> BST a.k.a. bst.hpp <|                                                   *
 *                                                                            *
 * "To find your true self, you may need to go back to your root(s)"          *
 *                                                                            *
 *                                               (on how `end` is returned)   *
 *                                                                            *
\******************************************************************************/

/**
 * @file node.hpp
 * @author Leonardo Arrighi
 * @author Francesco Romor
 * @author Emanuele Ballarin
 * @date 18 February 2020
 * @brief Header file containing the implementation of a type-templated binary search tree (bst), whith auxiliary functions.
 */

#pragma once  // Easy ODR

#include "iterator.hpp"
#include "node.hpp"

#include <cassert>
#include <cmath>
#include <iostream>  // Overload of operator<<
#include <queue>     // Use: std::queue
#include <utility>
#include <vector>

template<typename kt, typename vt, typename cmp = std::less<kt>>
class bst
{
    public:
    using key_type = const kt;
    using const_key_type = const kt;
    using const_value_type = const vt;
    using value_type = vt;
    using pair_type = std::pair<const kt, vt>;
    using node_type = Node<pair_type>;
    using iterator = tree_iterator<node_type, false>;
    using const_iterator = tree_iterator<node_type, true>;

    // default ctor
    inline bst() noexcept = default;

    // custom ctor with user-passed cmp
    inline bst(cmp c) : mycmp{c} {};

    // cpctor
    inline bst(const bst&);

    // mvctor
    inline bst(bst&&) noexcept = default;

    // cpasst
    inline bst& operator=(const bst&);

    //mvasst
    inline bst& operator=(bst&&) noexcept = default;


    //insert
    inline std::pair<iterator, bool> insert(const pair_type&);
    inline std::pair<iterator, bool> insert(pair_type&&);

    //emplace
    template<typename... Types>
    inline std::pair<iterator, bool> emplace(Types&&...);

    //clear
    inline void clear() noexcept;

    //(c)begin
    inline iterator begin();
    inline const_iterator begin() const;
    inline const_iterator cbegin() const;

    //(c)end
    inline iterator end();
    inline const_iterator end() const;
    inline const_iterator cend() const;

    //find
    inline iterator find(const key_type&);
    inline const_iterator find(const key_type&) const;

    //balance
    inline void balance();

    //subscripting
    inline value_type& operator[](const_key_type&);
    inline value_type& operator[](key_type&&);

    //Put-to operator
    friend std::ostream& operator<<(std::ostream& os, const bst<kt, vt, cmp>& bstree)
    {
        const_iterator iter{bstree.cbegin()};
        const char s = ' ';

        while (iter != bstree.cend())
        {
            //check that keys and values are printable
            os << "(" << iter->read_elem().first << ":" << iter->read_elem().second << ")" << s;
            ++iter;
        }
        // Retline
        return os;
    };

    //erase
    inline void erase(const_key_type&);

    private:
    cmp mycmp{};

    const node_type end_node{};
    std::unique_ptr<node_type> root;
    std::vector<iterator> vec;

    // Private swap function
    inline void swap(bst<kt, vt, cmp>&) noexcept;

    // Member comparison for keys
    inline bool ecmp(kt, kt);

    //helpers for balance
    inline void detach() noexcept;
    inline void balance_sub_l(std::size_t, std::size_t);  //Node::set
    inline void balance_sub_r(std::size_t, std::size_t);  //Node::set

    //helpers for erase
    inline void exchange(iterator);              //Node::set
    inline void replace(iterator);               //Node::set
    inline void substitute(iterator, iterator);  //Node::set
    inline void detach_leaf(iterator) noexcept;

    //helpers for begin and end
    inline iterator leftmost(node_type*) const noexcept;
    inline iterator rightmost(node_type*) const noexcept;
};


template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::clear() noexcept
{
    root.reset();
}

template<typename kt, typename vt, typename cmp>
inline bst<kt, vt, cmp>::bst(const bst<kt, vt, cmp>& original)
{
    if (!(original.root.get()))
    {
        this->root = nullptr;
    }
    else
    {
        this->root.reset(new Node<pair_type>(original.root.get()->read_elem()));

        std::queue<node_type*> queue_original{};
        std::queue<node_type*> queue_this{};

        queue_original.push(original.root.get());
        queue_this.push(this->root.get());

        // Copy-elision by hand
        while (!queue_original.empty())
        {
            // Sx case
            if (queue_original.front()->read_lc().get())
            {
                queue_this.front()->set_lc(new node_type(queue_original.front()->read_lc()->read_elem()));
                queue_original.push(queue_original.front()->read_lc().get());
                queue_this.push(queue_this.front()->read_lc().get());
            }

            // Dx case
            if (queue_original.front()->read_rc().get())
            {
                queue_this.front()->set_rc(new node_type(queue_original.front()->read_rc()->read_elem()));
                queue_original.push(queue_original.front()->read_rc().get());
                queue_this.push(queue_this.front()->read_rc().get());
            }

            // Pop after push, to allow an easy end condition
            queue_original.pop();
            queue_this.pop();
        }
    }
};

// cpasst
template<typename kt, typename vt, typename cmp>
inline bst<kt, vt, cmp>& bst<kt, vt, cmp>::operator=(const bst<kt, vt, cmp>& original)
{
    if (this != &original)  // Optimize against self-assignment
    {
        bst<kt, vt, cmp> copy = bst<kt, vt, cmp>(original);
        this->swap(copy);
    }
    return *this;
};


template<typename kt, typename vt, typename cmp>
inline std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(const pair_type& pair)
{
    std::pair<iterator, bool> to_be_ret = std::pair<iterator, bool>();
    node_type* cursor = this->root.get();

    if (!cursor)
    {
        root.reset(new node_type{pair});
        iterator found_key = iterator(this->root.get());
        to_be_ret.first = found_key;
        to_be_ret.second = true;
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (node_type* r_child = cursor->read_rc().get(); (mycmp(cursor_key, pair.first)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (mycmp(pair.first, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, pair.first))
                {
                    iterator found_key = iterator(cursor);
                    to_be_ret.first = found_key;
                    to_be_ret.second = false;
                    break;
                }
                else
                {
                    if (mycmp(cursor_key, pair.first))
                    {
                        cursor->set_rc(new node_type(pair));
                        iterator found_key = iterator(cursor->read_rc().get());
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    else
                    {
                        cursor->set_lc(new node_type(pair));
                        iterator found_key = iterator(cursor->read_lc().get());
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    break;
                }
            }
        }
    }
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
inline std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(pair_type&& pair)
{
    std::pair<iterator, bool> to_be_ret = std::pair<iterator, bool>();
    node_type* cursor = this->root.get();

    if (!cursor)
    {
        root.reset(new node_type{pair});
        iterator found_key = iterator(this->root.get());
        to_be_ret.first = found_key;
        to_be_ret.second = true;
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (node_type* r_child = cursor->read_rc().get(); (mycmp(cursor_key, pair.first)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (mycmp(pair.first, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, pair.first))
                {
                    iterator found_key = iterator(cursor);
                    to_be_ret.first = found_key;
                    to_be_ret.second = false;
                    break;
                }
                else
                {
                    if (mycmp(cursor_key, pair.first))
                    {
                        cursor->set_rc(new node_type(pair));
                        iterator found_key = iterator(cursor->read_rc().get());
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    else
                    {
                        cursor->set_lc(new node_type(pair));
                        iterator found_key = iterator(cursor->read_lc().get());
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    break;
                }
            }
        }
    }
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::begin()
{
    return leftmost(this->root.get());
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::begin() const
{
    const_iterator iter{leftmost(this->root.get())};
    return iter;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cbegin() const
{
    const_iterator iter{leftmost(this->root.get())};
    return iter;
}


template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::leftmost(typename bst<kt, vt, cmp>::node_type* node) const
  noexcept
{
    if (node)
    {
        while (node->read_lc().get())
        {
            node = node->read_lc().get();
        }
    }

    iterator leftmost{node};
    return leftmost;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::end()
{
    return rightmost(this->root.get());
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::end() const
{
    const_iterator iter{rightmost(this->root.get())};
    return iter;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cend() const
{
    const_iterator iter{rightmost(this->root.get())};
    return iter;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::rightmost(typename bst<kt, vt, cmp>::node_type* node) const
  noexcept
{
    if (node)
    {
        while (node->read_rc().get())
        {
            node = node->read_rc().get();
        }
    }

    iterator rightmost{node};
    return ++rightmost;
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::balance()
{
    //check that the tree is not void
    assert(this->root);

    //initialize vec with iterators mantaining the tree order
    for (iterator Iter{this->begin()}; Iter != this->cend(); ++Iter)
    {
        //vec.push_back(iterator{*Iter});
        vec.push_back(iterator(Iter));
    }

    //erase all relationships among the tree nodes
    this->detach();

    //set the new root iterator
    std::size_t middle_index = static_cast<std::size_t>(std::floor(vec.size() / 2.0));
    this->root.reset(&(*(vec[middle_index])));
    iterator new_root = vec[middle_index];

    //dividi et impera
    balance_sub_l(0, middle_index);
    balance_sub_r(middle_index + 1, vec.size());
    vec.clear();
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::detach() noexcept
{
    this->root.release();
    for (iterator mynode_iter : this->vec)
    {
        mynode_iter->detach_children();
    }
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::balance_sub_l(std::size_t left, std::size_t middle)
{
    std::size_t dis_ml = middle - left;

    auto l_child = left + static_cast<std::size_t>(std::floor(dis_ml / 2.0));
    vec[middle]->set_lc(&(*vec[l_child]));

    if (l_child - left > 0)
    {
        balance_sub_l(left, l_child);
    }
    if (middle - l_child > 1)
    {
        balance_sub_r(++l_child, middle);
    }
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::balance_sub_r(std::size_t middle, std::size_t right)
{
    std::size_t dis_rm = right - middle;

    auto r_child = middle + static_cast<std::size_t>(std::floor(dis_rm / 2.0));
    vec[middle - 1]->set_rc(&(*vec[r_child]));

    if (r_child - middle > 0)
    {
        balance_sub_l(middle, r_child);
    }
    if (right - r_child > 1)
    {
        balance_sub_r(++r_child, right);
    }
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::const_key_type& x)
{
    iterator found = this->find(x);
    return found->template value_refrw<key_type, vt>();
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::key_type&& x)
{
    iterator found = this->find(x);
    return found->template value_refrw<key_type, vt>();
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::erase(typename bst<kt, vt, cmp>::const_key_type& x)
{
    iterator erasing = find(x);

    if (erasing->read_lc() && erasing->read_rc())  //both children present
    {
        iterator next{erasing.next()};
        exchange(next);
        substitute(erasing, next);
        delete &*erasing;
    }
    else if (erasing->read_lc() || erasing->read_rc())  //only one child
    {
        replace(erasing);
        delete &*erasing;
    }
    else  //(!(erasing->read_lc()) && !(erasing->read_rc()))
    {
        detach_leaf(erasing);
        delete &*erasing;
    }
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::exchange(typename bst<kt, vt, cmp>::iterator actual)
{
    if (actual->read_lc() || actual->read_rc())  //only one child
    {
        replace(actual);
    }
    else  //(!actual->read_lc() && !actual->read_rc())
    {
        detach_leaf(actual);
    }
}

/* Erase a node with only one child. Two nested if-clauses: the first checks
if the node to be erased is left or right, the second checks if its child is
left or right. There are four cases in total*/
template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::replace(typename bst<kt, vt, cmp>::iterator substituting)
{
    if (substituting->read_pr())
    {
        if (substituting->is_left())
        {
            //more efficient way?
            iterator father{substituting->read_pr().get()};
            substituting->read_pr()->detach_left();
            if (substituting->read_lc())
            {
                iterator left{substituting->read_lc().get()};
                substituting->detach_left();
                father->set_lc(&*left);
            }
            else  //substituting has right node
            {
                iterator right{substituting->read_rc().get()};
                substituting->detach_right();
                father->set_lc(&*right);
            }
        }
        else
        {
            //more efficient way?
            iterator father{substituting->read_pr().get()};
            substituting->read_pr()->detach_right();

            if (substituting->read_lc())
            {
                iterator left{substituting->read_lc().get()};
                substituting->detach_left();
                father->set_rc(&*left);
            }
            else  //substituting has right node
            {
                iterator right{substituting->read_rc().get()};
                substituting->detach_right();
                father->set_rc(&*right);
            }
        }
    }
    else
    {
        if (substituting->read_lc())
        {
            iterator left{substituting->read_lc().get()};
            substituting->detach_left();
            root.release();
            root.reset(&*left);  //destroys previous root
        }
        else
        {
            iterator right{substituting->read_rc().get()};
            substituting->detach_right();
            root.release();
            root.reset(&*right);  //destroys previous root
        }
    }
}

/**The node to_be_substituted has two children, substituting
has no children and parent. At the end of the procedure to_be_substituted is
detached and needs to be erased or reattached properly.*/
template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::substitute(typename bst<kt, vt, cmp>::iterator to_be_substituted,
                                         typename bst<kt, vt, cmp>::iterator substituting)
{
    iterator left{to_be_substituted->read_lc().get()};
    iterator right{to_be_substituted->read_rc().get()};
    to_be_substituted->detach_children();

    if (to_be_substituted->read_pr())  //if to_be_substituted is not the root
    {
        iterator parent{to_be_substituted->read_pr().get()};
        if (to_be_substituted->is_left())
        {
            to_be_substituted->read_pr()->detach_left();
            parent->set_lc(&*substituting);
        }
        else
        {
            to_be_substituted->read_pr()->detach_right();
            parent->set_rc(&*substituting);
        }
        substituting->set_lc(&*left);
        substituting->set_rc(&*right);
    }
    else
    {
        substituting->set_lc(&*left);
        substituting->set_rc(&*right);
        this->root.release();
        this->root.reset(&*substituting);
    }
}

template<typename kt, typename vt, typename cmp>
inline void bst<kt, vt, cmp>::detach_leaf(typename bst<kt, vt, cmp>::iterator erasing) noexcept
{
    if (erasing->read_pr())
    {
        if (erasing->is_left())
        {
            erasing->read_pr()->detach_left();
        }
        else
        {
            erasing->read_pr()->detach_right();
        }
    }
    else
    {  //single root to erase
        root.release();
    }
}


template<typename kt, typename vt, typename cmp>
template<typename... Types>
inline std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::emplace(Types&&... args)
{
    std::pair<iterator, bool> to_be_ret = std::pair<iterator, bool>();
    to_be_ret = insert(std::move(pair_type(std::forward<Types>(args)...)));
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(const key_type& key)
{
    node_type* cursor = this->root.get();
    iterator iter_ret = iterator(cursor);

    if (!cursor)
    {
        iter_ret = end();
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (node_type* r_child = cursor->read_rc().get(); (mycmp(cursor_key, key)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (node_type* l_child = cursor->read_lc().get(); (mycmp(key, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, key))
                {
                    iterator found_key = iterator(cursor);
                    iter_ret = found_key;
                    break;
                }
                else
                {
                    iter_ret = end();
                    break;
                }
            }
        }
    }
    return iter_ret;
}

template<typename kt, typename vt, typename cmp>
inline typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::find(const key_type& key) const
{
    const_iterator iter{find(key)};
    return iter;
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::swap(bst<kt, vt, cmp>& given) noexcept
{
    if (*this != given)
    {
        this->root.swap(given.root);
    }
}

// User provided cmp-equality
template<typename kt, typename vt, typename cmp>
bool bst<kt, vt, cmp>::ecmp(kt lhs, kt rhs)
{
    return (!mycmp(lhs, rhs) && !mycmp(rhs, lhs));
}
