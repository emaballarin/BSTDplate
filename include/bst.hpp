#pragma once
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>
#include <queue>
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
    bst& operator=(bst&);

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

    //begin
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    //end
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

        bool has_children;

        while (!queue_original.empty())
        {
            has_children = (queue_original.front()->read_lc()->get()->read_lc()->get()
                            || queue_original.front()->read_rc()->get()->read_rc()->get());
            // Sx
            if (queue_original.front()->read_lc()->get())
            {
                queue_this.front()->set_lc(new node_type(queue_original.front()->read_lc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_lc()->get());
                    queue_this.push(queue_this.front()->read_lc()->get());
                }
            }

            // Dx
            if (queue_original.front()->read_rc()->get())
            {
                queue_this.front()->set_rc(new node_type(queue_original.front()->read_rc()->read_elem()));
                if (has_children)
                {
                    queue_original.push(queue_original.front()->read_rc()->get());
                    queue_this.push(queue_this.front()->read_rc()->get());
                }
            }

            queue_original.pop();
            queue_this.pop();
        }
    }
};

// cpasst
template<typename kt, typename vt, typename cmp>
bst<kt, vt, cmp>& bst<kt, vt, cmp>::operator=(bst<kt, vt, cmp>& original)
{
    bst<kt, vt, cmp> copy{original};
    this = &copy;
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
