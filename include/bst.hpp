#pragma once
#include "iterator.hpp"
#include "node.hpp"

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <queue>

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

    void exchange(iterator);
    void replace(iterator);
    void substitute(iterator, iterator);
    void detach_leaf(iterator);

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
typename bst<kt, vt, cmp>::iterator leftmost(typename bst<kt, vt, cmp>::node_type* node){
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
typename bst<kt, vt, cmp>::iterator rightmost(typename bst<kt, vt, cmp>::node_type* node){
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

  //erase all relationships among the tree nodes
  detach();

  //set the new root
  std::size_t middle_index = std::floor(vec.size()/2);
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
  return found->read_elem().second();//not range checked
}

template<typename kt, typename vt, typename cmp>
typename bst<kt, vt, cmp>::value_type&
bst<kt, vt, cmp>::operator[](typename bst<kt, vt, cmp>::key_type&& x){
  iterator found = this->find(x);
  return found->read_elem().second();//not range checked
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::erase(typename bst<kt, vt, cmp>::const_key_type& x){
  iterator erasing = find(x);

  //add exception
  if (erasing->read_lc() && erasing->read_rc())//both children present
  {
    iterator next{erasing.next()};
    exchange(next);
    substitute(erasing,next);
    delete *erasing;

  } else if(erasing->read_lc() || erasing->read_rc())//only one child
  {
    replace(erasing);
    delete *erasing;

  } else //(!(erasing->read_lc()) && !(erasing->read_rc()))
  {
    detach_leaf(erasing);
    delete *erasing;
  }

}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::exchange(typename bst<kt, vt, cmp>::iterator actual)
{
  if (actual->read_lc() && actual->read_rc())//both children present
  {
    iterator next{actual.next()};
    exchange(next);
    substitute(actual, next);

  } else if(actual->read_lc() || actual->read_rc())//only one child
  {
    replace(actual);

  } else //(!actual->read_lc() && !actual->read_rc())
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
  if (substituting->is_left())
  {
    //?more efficient way?
    iterator father{substituting->read_pr().get()};
    substituting->read_pr()->detach_left();
    if (substituting->read_lc())
    {
      iterator left{substituting->read_lc().get()};
      father->set_lc(*left);
    } else //substituting has right node
    {
      iterator right{substituting->read_rc().get()};
      father->set_lc(right);
    }
  } else
  {
    //?more efficient way?
    iterator father{substituting->read_pr().get()};
    substituting->read_pr()->detach_right();

    if (substituting->read_rc())
    {
      iterator left{substituting->read_lc().get()};
      father->set_rc(*left);
    } else //substituting has right node
    {
      iterator right{substituting->read_rc().get()};
      father->set_rc(right);
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
  iterator parent{to_be_substituted->read_pr()};
  iterator left{to_be_substituted->read_lc()};
  iterator right{to_be_substituted->read_rc()};

  to_be_substituted->detach_children();
  if (to_be_substituted->is_left()){
    to_be_substituted->read_pr()->detach_left();
    parent->set_lc(*substituting);
  }
  else {
    to_be_substituted->read_pr()->detach_right();
    parent->set_rc(*substituting);
  }

  substituting->set_lc(*left);
  substituting->set_rc(*left);
}

template<typename kt, typename vt, typename cmp>
void bst<kt, vt, cmp>::detach_leaf(typename bst<kt, vt, cmp>::iterator erasing)
{
  if (erasing->is_left()){ erasing->read_pr()->detach_left();}
  else{erasing->read_pr()->detach_right();}
}
