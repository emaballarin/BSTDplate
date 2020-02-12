#pragma once
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

template<typename kt, typename vt, typename cmp = std::less<kt>>
class bst
{
    public:
    using key_type = kt;
    using const_key_type = const kt;
    using const_value_type = const vt;
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
    std::unique_ptr<node_type> root;
    std::vector<iterator> vec;

    void detach();
    void balance_sub_l(std::size_t, std::size_t);
    void balance_sub_r(std::size_t, std::size_t);

};

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::clear()
{
  root.reset();
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
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cbegin() const{
  return const_iterator{leftmost(this->root.get())};
}

//I would use const_iterator inside leftmost, but then how to convert it in iterator?const_cast?
template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator leftmost(typename bst<kt, vt, cmp>::node_type*& node){
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
typename bst<kt, vt, cmp>::const_iterator bst<kt, vt, cmp>::cend() const{
  return const_iterator(rightmost(this->root.get()));
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::iterator rightmost(typename bst<kt, vt, cmp>::node_type*& node){
  typename bst<kt, vt, cmp>::iterator begin{node};

  while (!(begin->is_right()))
  {
    *begin = begin->read_rc().get();
  }

  return begin;
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::balance(){
  //check that the tree is not void

  //initialize vec with iterators mantaining the tree order
  for(iterator Iter{begin()}; Iter!=cend(); ++Iter){
    vec.push_back(iterator{*Iter});
  }

  //set the new root
  std::size_t middle_index = std::floor(vec.size()/2);
  iterator new_root = vec[middle_index];
  detach(new_root);

  //dividi et impera
  balance_sub_l(0, middle_index);
  balance_sub_r(middle_index, vec.size());

}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::detach()
{
  this->root->detach_left();
  this->root->detach_right();
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::balance_sub_l(std::size_t left, std::size_t middle)
{
  std::size_t dis_ml = middle-left;

  std::size_t l_child = left+std::floor(dis_ml/2);
  detach(vec[l_child]);
  vec[middle]->set_lc(*vec[l_child]);

  if(l_child-left>0){ balance_sub_l(left, l_child);}
  if(middle-l_child>1){ balance_sub_r(++l_child, middle);}

}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::balance_sub_r(std::size_t middle, std::size_t right)
{
  std::size_t dis_rm = right-middle;

  std::size_t r_child = right-std::floor(dis_rm/2);
  detach(vec[r_child]);
  vec[middle]->set_rc(*vec[r_child]);

  if(r_child-middle>0){balance_sub_l(middle, r_child);}
  if(right-r_child>1){balance_sub_l(++r_child, right);}
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type&
bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::const_key_type& x){
  iterator found = this->find(x);
  //add assert?
  return found->read_elem().second();//not range checked
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type&
bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::key_type&& x){
  iterator found = this->find(x);
  //add assert?
  return found->read_elem().second();//not range checked
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::erase(typename bst<kt, vt, cmp>::const_key_type& x){
  iterator erasing = find(x);
  //check to_be_erased is not end
  //check special cases erasing root
  if (erasing->read_lc() && erasing->read_rc()){//both children
    iterator next{erasing.next()};
    substitute(erasing, next);
  } else if(erasing->read_lc() || erasing->read_rc()){//XOR with previous if condition
    substitute(erasing, erasing->read_pr());
  } else (!erasing->read_lc() && !erasing->read_rc()){//leaf

  }

}
