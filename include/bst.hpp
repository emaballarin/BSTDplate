#pragma once
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>
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

    bst() = default;

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
    friend std::ostream& operator<<(std::ostream& os, const bst& x);

    //erase
    void erase(const key_type& x);

    private:
    Node<pair_type>* root;
    std::vector<node_type*> ordered_vec;

    //wrapper for find: bool=true returns end, bool=false returns where to insert
    iterator find_private(const key_type&, bool);
    const_iterator find_private(const key_type&, bool) const;
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
