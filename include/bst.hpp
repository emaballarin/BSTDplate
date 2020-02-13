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
#include <iostream>  // Overload of operator<<
#include <queue>     // Use: std::queue
#include <utility>
#include <vector>

template<typename kt, typename vt, typename cmp = std::less<kt>>
class bst
{
    public:
    using key_type = kt;
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
    std::pair<iterator, bool> insert(const pair_type& x);
    std::pair<iterator, bool> insert(pair_type&& x);

    //emplace
    template<class... Types>
    std::pair<iterator, bool> emplace(Types&&... args);

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
    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;

    // find, overloaded on RVALUE
    iterator find(key_type&& x);

    //balance
    void balance();

    //subscripting
    value_type& operator[](const key_type& x);
    value_type& operator[](key_type&& x);

    //Put-to operator
    friend std::ostream& operator<<(std::ostream& os, const bst<kt, vt, cmp>& x)
    {
        // Do stuff here!
        return os;
    };

    //erase
    void erase(const key_type& x);

    private:
    std::unique_ptr<node_type> root;
    //node_type* root;
    std::vector<node_type*> ordered_vec;

    //wrapper for find: bool=true returns end, bool=false returns where to insert
    iterator find(const key_type&, bool);
    const_iterator find(const key_type&, bool) const;

    iterator find_private(const key_type&);
    const_iterator find_private(const key_type&) const;

    // Private swap function
    void swap(bst<kt, vt, cmp>&) noexcept;

    // Member comparison for keys
    bool ecmp(kt, kt);
};


// cpctor
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

        bool has_children;  // Hoist-by-hand

        // Copy-elision by hand: so much fun! :)
        while (!queue_original.empty())
        {
            has_children = (queue_original.front()->read_lc().get()->read_lc().get()
                            || queue_original.front()->read_rc().get()->read_rc().get());  // Avoid repeated evaluation
            // Sx case
            if (queue_original.front()->read_lc().get())
            {
                queue_this.front()->set_lc(new node_type(queue_original.front()->read_lc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_lc().get());
                    queue_this.push(queue_this.front()->read_lc().get());
                }
            }

            // Dx case
            if (queue_original.front()->read_rc().get())
            {
                queue_this.front()->set_rc(new node_type(queue_original.front()->read_rc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_rc().get());
                    queue_this.push(queue_this.front()->read_rc().get());
                }
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
        //this = &copy;
        this->swap(copy);
    }
    return *this;
};


template<typename kt, typename vt, typename cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(const pair_type& pair)
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    auto& cursor_key = cursor->read_elem().first;
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret;

    while (true)
    {
        if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get();
            (cmp(pair.first, cursor_key)) && (r_child))
        {
            cursor = r_child;
            cursor_key = cursor->read_elem().first;
        }
        else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                 (cmp(cursor_key, pair.first)) && (l_child))
        {
            cursor = l_child;
            cursor_key = cursor->read_elem().first;
        }
        else
        {
            if (ecmp(cursor_key, pair.first))
            {
                tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor};
                to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                break;
            }
            else
            {
                if (cmp(cursor_key, pair.first))
                {
                    cursor->set_rc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_rc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                else
                {
                    cursor->set_lc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_lc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                break;
            }
        }
    }
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(pair_type&& pair)
// COPY OF THE ABOVE
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    auto& cursor_key = cursor->read_elem().first;
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret;

    while (true)
    {
        if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get();
            (cmp(cursor_key, pair.first)) && (r_child))
        {
            cursor = r_child;
            cursor_key = cursor->read_elem().first;
        }
        else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                 (cmp(cursor_key, pair.first)) && (l_child))
        {
            cursor = l_child;
            cursor_key = cursor->read_elem().first;
        }
        else
        {
            if (ecmp(cursor_key, pair.first))
            {
                tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor};
                to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                break;
            }
            else
            {
                if (cmp(cursor_key, pair.first))
                {
                    cursor->set_rc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_rc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                else
                {
                    cursor->set_lc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_lc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                break;
            }
        }
    }
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
template<class... Types>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::emplace(Types&&... args)
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    auto& cursor_key = cursor->read_elem().first;
    std::pair<bst<kt, vt, cmp>::iterator, bool> to_be_ret;

    // Check for correctness, eventually.
    std::pair pair = std::forward<std::pair<kt, vt>>(args...);

    while (true)
    {
        if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get();
            (cmp(pair.first, cursor_key)) && (r_child))
        {
            cursor = r_child;
            cursor_key = cursor->read_elem().first;
        }
        else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get();
                 (cmp(cursor_key, pair.first)) && (l_child))
        {
            cursor = l_child;
            cursor_key = cursor->read_elem().first;
        }
        else
        {
            if (ecmp(cursor_key, pair.first))
            {
                cursor->write_elem(pair);
                tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor};
                to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, false};
                break;
            }
            else
            {
                if (cmp(cursor_key, pair.first))
                {
                    cursor->set_rc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_rc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                else
                {
                    cursor->set_lc(new node_type(pair));
                    tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor->read_lc().get()};
                    to_be_ret = std::pair<bst<kt, vt, cmp>::iterator, bool>{found_key, true};
                }
                break;
            }
        }
    }
    return to_be_ret;
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(const key_type& key)
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    auto& cursor_key = cursor->read_elem().first;
    tree_iterator<Node<std::pair<kt, vt>>, false> iter_ret{cursor};

    while (true)
    {
        if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get(); (cmp(key, cursor_key)) && (r_child))
        {
            cursor = r_child;
            cursor_key = cursor->read_elem().first;
        }
        else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get(); (cmp(key, cursor_key)) && (l_child))
        {
            cursor = l_child;
            cursor_key = cursor->read_elem().first;
        }
        else
        {
            if (ecmp(cursor_key, key))
            {
                tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor};
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
    return iter_ret;
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(key_type&& key)
{
    Node<std::pair<const kt, vt>>* cursor = this->root.get();
    auto& cursor_key = cursor->read_elem().first;
    tree_iterator<Node<std::pair<kt, vt>>, false> iter_ret{cursor};

    while (true)
    {
        if (Node<std::pair<const kt, vt>>* r_child = cursor->read_rc().get(); (cmp(key, cursor_key)) && (r_child))
        {
            cursor = r_child;
            cursor_key = cursor->read_elem().first;
        }
        else if (Node<std::pair<const kt, vt>>* l_child = cursor->read_lc().get(); (cmp(key, cursor_key)) && (l_child))
        {
            cursor = l_child;
            cursor_key = cursor->read_elem().first;
        }
        else
        {
            if (ecmp(cursor_key, key))
            {
                tree_iterator<Node<std::pair<kt, vt>>, false> found_key{cursor};
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
    return iter_ret;
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::find(const key_type& key) const
{
    // ??
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
    return (!cmp(lhs, rhs) && !cmp(rhs, lhs));
}
