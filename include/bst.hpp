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
    iterator find_private(const key_type&, bool);
    const_iterator find_private(const key_type&, bool) const;
};


// cpctor
template<typename kt, typename vt, typename cmp>
bst<kt, vt, cmp>::bst(const bst<kt, vt, cmp>& original)
{
    if (!(original->root))
    {
        this->root = nullptr;
    }
    else
    {
        std::queue<node_type*> queue_original{};
        std::queue<node_type*> queue_this{};

        queue_original.push(original->root);
        queue_this.push(this->root);

        bool has_children;  // Hoist-by-hand

        // Copy-elision by hand: so much fun! :)
        while (!queue_original.empty())
        {
            has_children =
              (queue_original.front()->read_lc()->get()->read_lc()->get()
               || queue_original.front()->read_rc()->get()->read_rc()->get());  // Avoid repeated evaluation
            // Sx case
            if (queue_original.front()->read_lc()->get())
            {
                queue_this.front()->set_lc(new node_type(queue_original.front()->read_lc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_lc()->get());
                    queue_this.push(queue_this.front()->read_lc()->get());
                }
            }

            // Dx case
            if (queue_original.front()->read_rc()->get())
            {
                queue_this.front()->set_rc(new node_type(queue_original.front()->read_rc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_rc()->get());
                    queue_this.push(queue_this.front()->read_rc()->get());
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
        this = &copy;
    }

    return *this;
};


/*They are used to insert a new node. The function returns a pair of an iterator
(pointing to the node) and a bool. The bool is true if a new node has been
allocated, false otherwise (i.e., the key was already present in the tree).*/
template<typename kt, typename vt, typename cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(const pair_type& x)
{
}

template<typename kt, typename vt, typename cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(pair_type&& x)
{
}

template<typename kt, typename vt, typename cmp>
template<class... Types>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::emplace(Types&&... args)
{
    return std::pair<iterator, bool>();
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::clear()
{
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find(const key_type& x)
{
    //return bst::iterator(<#initializer #>);
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::find(const key_type& x) const
{
    //return bst::const_iterator(<#initializer #>);
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](const key_type& x)
{
    //return <#initializer #>;
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type& bst<kt, vt, cmp>::operator[](key_type&& x)
{
    //return <#initializer #>;
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::erase(const key_type& x)
{
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator bst<kt, vt, cmp>::find_private(const key_type&, bool)
{
    //return bst::iterator(<#initializer #>);
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::find_private(const key_type&, bool) const
{
    //return bst::const_iterator(<#initializer #>);
}
