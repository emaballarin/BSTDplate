/******************************************************************************\
 * |> BST a.k.a. bst.hpp <|                                                   *
 *                                                                            *
 * "To find your true self, you may need to go back to your root(s)"          *
 *                                                                            *
 *                                               (on how `end` is returned)   *
 *                                                                            *
\******************************************************************************/

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
    bst() = default;

    // cpctor
    bst(const bst&);

    // mvctor
    bst(bst&&) noexcept = default;

    // cpasst
    bst& operator=(const bst&);

    //mvasst
    bst& operator=(bst&&) noexcept = default;


    //insert
    std::pair<iterator, bool> insert(const pair_type&);
    std::pair<iterator, bool> insert(pair_type&&);

    //emplace
    template<class... Types>
    std::pair<iterator, bool> emplace(Types&&...);

    //clear
    void clear();

    //(c)begin
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    //(c)end
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    //find
    iterator find(const key_type&);
    const_iterator find(const key_type&) const;

    // find, overloaded on RVALUE
    iterator find(key_type&&);

    //balance
    void balance();

    //subscripting
    value_type& operator[](const_key_type&);
    value_type& operator[](key_type&&);

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
    void erase(const_key_type&);

    private:
    std::unique_ptr<node_type> root;
    std::vector<iterator> vec;

    // Private swap function
    void swap(bst<kt, vt, cmp>&) noexcept;

    // Member comparison for keys
    bool ecmp(kt, kt);

    void detach();
    void balance_sub_l(std::size_t, std::size_t);
    void balance_sub_r(std::size_t, std::size_t);

    void exchange(iterator);
    void replace(iterator);
    void substitute(iterator, iterator);
    void detach_leaf(iterator);
    iterator leftmost(node_type*);
};


template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::clear()
{
    root.reset();
}

template<typename kt, typename vt, typename cmp>
bst<kt, vt, cmp>::bst(const bst<kt, vt, cmp>& original)
{
    if (!(original.root.get()))
    {
        this->root = nullptr;
    }
    else
    {
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
bst<kt, vt, cmp>& bst<kt, vt, cmp>::operator=(const bst<kt, vt, cmp>& original)
{
    if (this != &original)  // Optimize against self-assignment
    {
        bst<kt, vt, cmp> copy{original};
        this->swap(copy);
    }
    return *this;
};


template<typename kt, typename vt, typename cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(const pair_type& pair)
{
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
    node_type* cursor = this->root.get();

    // IFELSE //
    if (!cursor)
    {
        // [FIX ATTEMPT 006] INSERT LVR ->
        root.reset(new node_type{pair});
        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
          tree_iterator<Node<std::pair<key_type, value_type>>, false>(this->root.get());
        to_be_ret.first = found_key;
        to_be_ret.second = true;
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (node_type* r_child = cursor->read_rc().get(); (cmp()(cursor_key, pair.first)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (cmp()(pair.first, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, pair.first))
                {
                    tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                    to_be_ret.first = found_key;
                    to_be_ret.second = false;
                    break;
                }
                else
                {
                    if (cmp()(cursor_key, pair.first))
                    {
                        cursor->set_rc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_rc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    else
                    {
                        cursor->set_lc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_lc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
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
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(pair_type&& pair)
// COPY OF THE ABOVE
{
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
    node_type* cursor = this->root.get();

    // IFELSE //
    if (!cursor)
    {
        // [FIX ATTEMPT 006] INSERT RVR ->
        root.reset(new node_type{pair});
        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
          tree_iterator<Node<std::pair<key_type, value_type>>, false>(this->root.get());
        to_be_ret.first = found_key;
        to_be_ret.second = true;
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (node_type* r_child = cursor->read_rc().get(); (cmp()(cursor_key, pair.first)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (cmp()(pair.first, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, pair.first))
                {
                    tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                    to_be_ret.first = found_key;
                    to_be_ret.second = false;
                    break;
                }
                else
                {
                    if (cmp()(cursor_key, pair.first))
                    {
                        cursor->set_rc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_rc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    else
                    {
                        cursor->set_lc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_lc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
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
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::begin()
{
    return leftmost(this->root.get());
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::begin() const
{
    return const_iterator{leftmost(this->root.get())};
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cbegin() const
{
    return const_iterator{leftmost(this->root.get())};
}

//I would use const_iterator inside leftmost, but then how to convert it in iterator?const_cast?
template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::leftmost(typename bst<kt, vt, cmp>::node_type* node)
{
    typename bst<kt, vt, cmp>::iterator begin{node};

    while (!(begin->is_left()))
    {
        *begin = begin->read_lc().get();
    }

    return begin;
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::end()
{
    return rightmost(this->root.get());
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::end() const
{
    return const_iterator(rightmost(this->root.get()));
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cend() const
{
    return const_iterator(rightmost(this->root.get()));
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator rightmost(typename bst<kt, vt, cmp>::node_type* node)
{
    typename bst<kt, vt, cmp>::iterator begin{node};

    while (!(begin->is_right()))
    {
        *begin = begin->read_rc().get();
    }

    return begin;
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::balance()
{
    //check that the tree is not void

    //initialize vec with iterators mantaining the tree order
    for (iterator Iter{begin()}; Iter != cend(); ++Iter)
    {
        vec.push_back(iterator{*Iter});
    }

    //erase all relationships among the tree nodes
    detach();

    //set the new root
    std::size_t middle_index = std::floor(vec.size() / 2.0);
    iterator new_root = vec[middle_index];

    //dividi et impera
    balance_sub_l(0, middle_index);
    balance_sub_r(middle_index, vec.size());
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::detach()
{
    std::queue<node_type*> queue{};
    queue.push(this->root->get());

    while (!queue.empty())
    {
        // Sx case
        if (queue.front()->read_lc().get())
        {
            queue.push(queue.front()->read_lc().get());
        }

        // Dx case
        if (queue.front()->read_rc().get())
        {
            queue.push(queue.front()->read_rc().get());
        }

        queue.front()->detach_children();
        queue.pop();
    }
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::balance_sub_l(std::size_t left, std::size_t middle)
{
    std::size_t dis_ml = middle - left;

    auto l_child = left + static_cast<std::size_t>(std::floor(dis_ml / 2.0));
    detach(vec[l_child]);
    vec[middle]->set_lc(*vec[l_child]);

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
void bst<kt, vt, cmp>::balance_sub_r(std::size_t middle, std::size_t right)
{
    std::size_t dis_rm = right - middle;

    auto r_child = static_cast<std::size_t>(right - std::floor(dis_rm / 2.0));
    detach(vec[r_child]);
    vec[middle]->set_rc(*vec[r_child]);

    if (r_child - middle > 0)
    {
        balance_sub_l(middle, r_child);
    }
    if (right - r_child > 1)
    {
        balance_sub_l(++r_child, right);
    }
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::const_key_type& x)
{
    iterator found = this->find(x);
    return found->read_elem().second;  //not range checked
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::key_type&& x)
{
    iterator found = this->find(x);
    return found->read_elem().second;  //not range checked
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::erase(typename bst<kt, vt, cmp>::const_key_type& x)
{
    iterator erasing = find(x);

    //add exception
    if (erasing->read_lc() && erasing->read_rc())  //both children present
    {
        iterator next{erasing.next()};
        exchange(next);
        substitute(erasing, next);
        delete *erasing;
    }
    else if (erasing->read_lc() || erasing->read_rc())  //only one child
    {
        replace(erasing);
        delete *erasing;
    }
    else  //(!(erasing->read_lc()) && !(erasing->read_rc()))
    {
        detach_leaf(erasing);
        delete *erasing;
    }
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::exchange(typename bst<kt, vt, cmp>::iterator actual)
{
    if (actual->read_lc() && actual->read_rc())  //both children present
    {
        iterator next{actual.next()};
        exchange(next);
        substitute(actual, next);
    }
    else if (actual->read_lc() || actual->read_rc())  //only one child
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
void bst<kt, vt, cmp>::replace(typename bst<kt, vt, cmp>::iterator substituting)
{
    if (substituting->read_pr())
    {
        if (substituting->is_left())
        {
            //?more efficient way?
            iterator father{substituting->read_pr().get()};
            substituting->read_pr()->detach_left();
            if (substituting->read_lc())
            {
                iterator left{substituting->read_lc().get()};
                substituting->detach_left();
                father->set_lc(*left);
            }
            else  //substituting has right node
            {
                iterator right{substituting->read_rc().get()};
                substituting->detach_right();
                father->set_lc(right);
            }
        }
        else
        {
            //?more efficient way?
            iterator father{substituting->read_pr().get()};
            substituting->read_pr()->detach_right();

            if (substituting->read_rc())
            {
                iterator left{substituting->read_lc().get()};
                substituting->detach_left();
                father->set_rc(*left);
            }
            else  //substituting has right node
            {
                iterator right{substituting->read_rc().get()};
                substituting->detach_right();
                father->set_rc(right);
            }
        }
    }
    else
    {
        if (substituting->read_lc())
        {
            iterator left{substituting->read_lc().get()};
            substituting->detach_left();
            root->reset(*left);  //destroys previous root
        }
        else
        {
            iterator right{substituting->read_rc().get()};
            substituting->detach_right();
            root->reset(*right);  //destroys previous root
        }
    }
}

/*Substitute overload: the node to_be_substituted has two children, substituting
has no children and parent. At the end of the procedure to_be_substituted is
detached and needs to be erased or reattached properly.*/
template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::substitute(typename bst<kt, vt, cmp>::iterator to_be_substituted,
                                  typename bst<kt, vt, cmp>::iterator substituting)
{
    iterator left{to_be_substituted->read_lc()};
    iterator right{to_be_substituted->read_rc()};
    to_be_substituted->detach_children();

    if (to_be_substituted->read_pr())  //if to_be_substituted is not the root
    {
        iterator parent{to_be_substituted->read_pr()};
        if (to_be_substituted->is_left())
        {
            to_be_substituted->read_pr()->detach_left();
            parent->set_lc(*substituting);
        }
        else
        {
            to_be_substituted->read_pr()->detach_right();
            parent->set_rc(*substituting);
        }
    }

    substituting->set_lc(*left);
    substituting->set_rc(*right);
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::detach_leaf(typename bst<kt, vt, cmp>::iterator erasing)
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
        root.reset();
    }
}


template<typename kt, typename vt, typename cmp>
template<class... Types>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::emplace(Types&&... args)
{
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
    Node<std::pair<const kt, vt>>* cursor = this->root.get();

    // Check for correctness, eventually.
    std::pair pair = std::forward<std::pair<kt, vt>>(args...);

    // IFELSE //
    if (!cursor)
    {
        // [FIX ATTEMPT 006] EMPLACE ->
        root.reset(new node_type{pair});
        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
          tree_iterator<Node<std::pair<key_type, value_type>>, false>(this->root.get());
        to_be_ret.first = found_key;
        to_be_ret.second = true;
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get();
                (cmp()(cursor_key, pair.first)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (cmp()(pair.first, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, pair.first))
                {
                    cursor->write_elem(pair);
                    tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                    //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                    to_be_ret.first = found_key;
                    to_be_ret.second = false;
                    break;
                }
                else
                {
                    if (cmp()(cursor_key, pair.first))
                    {
                        cursor->set_rc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_rc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
                        to_be_ret.first = found_key;
                        to_be_ret.second = true;
                    }
                    else
                    {
                        cursor->set_lc(new node_type(pair));
                        tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                          tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor->read_lc().get());
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                        //to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>();
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
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(const key_type& key)
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    tree_iterator<Node<std::pair<key_type, value_type>>, false> iter_ret =
      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);


    // IFELSE //
    if (!cursor)
    {
        // [FIX ATTEMPT 006] FIND NONCONST ->
        iter_ret = end();
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get(); (cmp()(key, cursor_key)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (cmp()(key, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, key))
                {
                    tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);
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
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(key_type&& key)
// COPY OF THE ABOVE
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    tree_iterator<Node<std::pair<key_type, value_type>>, false> iter_ret =
      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);


    // IFELSE //
    if (!cursor)
    {
        // [FIX ATTEMPT 006] FIND NONCONST ->
        iter_ret = end();
    }
    else
    {
        kt cursor_key = cursor->read_elem().first;

        while (true)
        {
            if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get(); (cmp()(key, cursor_key)) && (r_child))
            {
                cursor = r_child;
                cursor_key = cursor->read_elem().first;
            }
            else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                     (cmp()(key, cursor_key)) && (l_child))
            {
                cursor = l_child;
                cursor_key = cursor->read_elem().first;
            }
            else
            {
                if (ecmp(cursor_key, key))
                {
                    tree_iterator<Node<std::pair<key_type, value_type>>, false> found_key =
                      tree_iterator<Node<std::pair<key_type, value_type>>, false>(cursor);
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
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::find(const key_type& key) const
{
    return tree_iterator_constify(find(key));  // ?? does it work?
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::swap(bst<kt, vt, cmp>& given) noexcept
{
    given.root.swap(this->root);
}

// User provided cmp-equality
template<typename kt, typename vt, typename cmp>
bool bst<kt, vt, cmp>::ecmp(kt lhs, kt rhs)
{
    return (!cmp()(lhs, rhs) && !cmp()(rhs, lhs));
}
