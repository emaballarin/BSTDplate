# Advanced Programming Exam

The exam consists of a written project followed by an oral discussion. 
The written project is due to February 19, 2020, at 11:59 PM.
Orals take place on 19th and 20th of February.

- Work in groups make of two/three people.
- Use a separate git repository. 
- You have to upload all and only the **source files** you wrote, with a **Makefile** and a **readme.md** where you describe how to compile, run your code, and a short report on what you have done, understood, and, eventually, benchmarked(COMPARE PERFORMANCE WITH OTHER LIBRARIES).
- Your code must have no memory leaks.
- No warnings must appear if compiled with the flags `-Wall -Wextra`.
- Don't change the name of the functions(DO NOT)!
- Once finished, send me either the link to the repository or a `tar.gz` with the files (`.git` folder included).

## Binary search tree
A MAP IS SELF-BALANCING(SAME NODES ON LEFT AND RIGHT): EVERY TIME YOU ADD AN ELEMENT IT REBALANCES ITSELF

The project consists of the implementation of a **template** binary search tree (BST). A BST is a hierarchical (ordered) data structure where each **node** can have at most two children, namely, **left** and **right** child. Each node stores a **pair** of a **key** and the associated **value**. The binary tree is ordered according to the keys. 
If we assume that we sort the keys in ascending order (i.e., we use the less than `<` operator), then given a node `N`, all the nodes having keys **smaller** than the key of the node `N` are on the **left**. All the nodes with a key **greater** than the key of the node `N` are on the **right**.

![](./.aux/binary.png)


Practically speaking, given the binary tree in the picture, if you need to insert a new node with `key=5`, you start from the root node `8`, you go left since `5<8`, you reach node `3`, then you go right, you land in `6`, you go left reaching node `4`. Node `4` has no right child, so the new node `5` becomes the right child of node `4`. If a key is already present in the tree, the associated value **is not** changed (BUT YOU HAVE TO INFORM THE USER).

From the implementation point of view, a node has two pointers `left` and `right` pointing to the left and right child, respectively. The pointers point to `nullptr` if they have no children.

It is useful to add a pointer (head, root, whatever you like) pointing to the top node, called **root node**.

### Tree traversal

The tree must be traversed in order, i.e., if I "print" the tree in the picture, I expect to see on the screen the sequence
```
1 3 4 6 7 8 10 13 14
```
node `1` is the first node, and node `14` is the last one.

IF I CANOT GO RIGHT DOWN I GO UP RIGHT, IF I CAN GO RIGHT I GO RIGHT AND THEN AS LEFT AS I CAN. IF I CANNOT GO RIGHT I GO UP. PRIORITY: LEFT DOWN, RIGHT DOWN, UP.

### Assignments
You have to solve the following tasks using modern C++14 (C++17 is welcome as well).

- Implement a **template** binary search tree class, named `bst`.
- The class has three templates:
  - the key type
  - the value type associated with the key
  - the type of the comparison operator, which is used to compare two keys. `std::less<key_type>` should be used as default choice.(DO NOT USE NAKED COMPARISON OPERATOR: OVERLOAD <, >, ==, !=)

template< typename key, typename vt, typenmae cmp>
class bst{
cmp op;//operator
public:
bst(cmp x): op{x}{}

es.
template
struct less{
bool operator()(const T& x, const T& y){ return x< y}}

- Implement proper iterators for your tree (i.e., `iterator` and `const_iterator`). Forward iterator is sufficient. (THE ITERATOR SHOULD FOLLOW THE ORTHER AS IN TREE TRAVERSAL)
- Mark `noexcept` the right functions.
- Remember the KISS principle. 
- Implement at least the following public member functions.

##### Insert

```c++
std::pair<iterator, bool> insert(const pair_type& x);
std::pair<iterator, bool> insert(pair_type&& x);
```
They are used to insert a new node. The function returns a pair of an iterator (pointing to the node) and a bool. The bool is true if a new node has been allocated, false otherwise (i.e., the key was already present in the tree). `pair_type` can be for example `std::pair<const key_type, value_type>`. 

t.insert({1,2});//make it like below
//t.insert(std::pair<kt, vt>{1,2});
t.emplace(5, 4);

##### Emplace

```c++
template< class... Types >
std::pair<iterator,bool> emplace(Types&&... args);
```
Inserts a new element into the container constructed in-place with the given args if there is no element with the key in the container.
HINT: THE KEY IS CONST

##### Clear

```c++
void clear();
```
Clear the content of the tree.

##### Begin

```c++
iterator begin();
const_iterator begin() const;
const_iterator cbegin() const;
```

Return an iterator to the left-most node (which, likely, is not the root node).
WHEN YOU DEREFERENCE A CONST ITERATOR YOU GET A CONST OBJECT (READ-ONLY)

##### End

```c++
iterator end();
const_iterator end() const;
const_iterator cend() const;
```

Return an iterator to one-past(NB ONE PAST) the last element.

##### Find

```c++
iterator find(const key_type& x);
const_iterator find(const key_type& x) const;
```
Find a given key. If the key is present, returns an iterator to the proper node, `end()` otherwise.

##### Balance

```c++
void balance();
```

Balance the tree. A simple and naive implementation is fine. The aim of this exam is not producing a super-performant code, but learning c++. No extra points for complicated algorithms(STILL OPEN RESEARCH FIELD).
TAKE THE ORDERED LIST AS IN TRASVERSAL, CLEAR AND THEN RECONSTRUCT IN THE MOST OBVIOUS WAY CUTTING IN HALF.

##### Subscripting operator

```c++
value_type& operator[](const key_type& x);
value_type& operator[](key_type&& x);
```
Returns a reference to the value that is mapped to a key equivalent to `x`, performing an insertion if such key does not already exist.

NB: THERE IS NO CONST BEFORE{: 

insert({x, vt{}})// for the default constructor

##### Put-to operator

```c++
friend
std::ostream& operator<<(std::ostream& os, const bst& x);//instead of bst<,,,>
```

Implement the friend function **inside** the class, such that you do not have to specify the templates for `bst`.

##### Copy and move

The copy semantics perform a deep-copy. Move semantics is as usual.

##### Erase

```c++
void erase(const key_type& x);
```

Removes the element (if one exists) with the key equivalent to key.
IT IS IMPORTANT THAT YOU DO NOT DELETE OTHER NODES, CHANGE THE KEY (FOR EXAMPLE FROM 3 TO 4)[CHIEDI]

### Hints
- **Big hint** use `std::pair<const key_type,value_type>`, which is defined in the header `utility`
- start coding and using the iterators ASAP.
-CLASSES:BST, ITERATOR (AND CONST ITERATOR, DEPENDS)[ITERATOR INSIDE CLASS BST??THIS WOULD BE RIGHT IF THEY HAVE THE SAME TEMPLATES] AND NODE(WHICH STORES THE PAIR [NUMBER OF TEMPLATES?? THE LESS THE BETTER])

template<typename T>//NB NODE COULD BE AN INDEPENDENT CONCEPT SO IT MAKES SENSE TO HAVE JUST ONE NODE
struct Node{
using value_type=T
left pointer (unique pointer)
righ pointer (unique pointer)
father pointer (shared pointer)
}

template<,,typename cmp=std::less<kt>(default)>
class bst{
using node_type=Node<std::pair<key_type, value_type>>;//
using iterator=_iterator<node_type, typename node_type::value_type>
using const_iterator=_iterator<node_type, const T, "">
}

template<typename node>
class iterator{
Node* current
using value_type= typename node::value_type// we use typename to avoid the compiler to think of it as the name of a static variable of node
using reference=value_type&
using pointer=value_type*
using iterator_category=std::??
using difference_type=std::ptrdiff_type;
return current;
};

template<typename node>
class const_iterator{
Node* const current
using value_type= typename node::value_type// we use typename to avoid the compiler to think of it as the name of a static variable of node
using reference=value_type&
using pointer=value_type*
using iterator_category=std::??
using difference_type=std::ptrdiff_type;
return current;
};

template<typename node, typename T>
class _iterator{
Node* const current
using value_type= typename node::value_type// we use typename to avoid the compiler to think of it as the name of a static variable of node
using reference=value_type&
using pointer=value_type*
using iterator_category=std::??
using difference_type=std::ptrdiff_type;
return current;
};//avoid code duplication for const_iterator

reference operator*() const{
return??
}

//in the header
std::pair<iterator, bool> insert(const kt& x);

//written outside class
template<kt, vt, cmp>
std::pair<typename bst<kt, vt, cmp>::iterator, bool> bst<kt, vt, cmp>::insert(const kt& x);


ITERATOR IS A POINTER TO A PAIR OF A REFERENCES(KEY, VALUE)
CONST_ITERATOR IS A POINTER TO A PAIR OF CONST_REFERENCE(KEY, VALUE)


FILES
bst hpp
#pragma once(same as #ifndef #endif)(without breaking the one-definition rule)
template<>
class bst{}
#include "bits_bst.hpp"

#to test
bits_bst.hpp
#ifndef 
#include "bst.hpp"

insert..
#endif

NB keep templates in headers

