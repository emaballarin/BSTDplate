#include <node.hpp>
#include <utility>


template<typename kt, typename vt, typename Cmp = std::less<kt>(default)>
class bst
{
    public:

    using key_type = kt;
    using value_type = vt;
    using node_type = Node<std::pair<key_type, value_type>>;
    using iterator = iterator<node_type>;
    //using const_iterator

    friend class iterator<node_type>;

    bst() = default;
    bst

      //insert
      std::pair<iterator, bool>
      insert(const pair_type& x);
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
    Node* root;
};
