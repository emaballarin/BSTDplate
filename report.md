# Advanced Programming Exam Project: *Implementation of a template BST in modern C++*

Submitted by [Leonardo Arrighi](https://github.com/LeonardoArrighi), [Francesco Romor](https://github.com/FrancescoRo) and [Emanuele Ballarin](https://github.com/emaballarin).

## Introduction

The present report, submitted in fulfilment of the requirements for the *Advanced Programming exam* offered by [SISSA](https://sissa.it), describes and further documents (in addition to *Doxygen* documentation and additional code comments) the structure, main design choices, testing and benchmarking procedures and results of the design and implementation of a *Template Binary Search Tree* (BST) – complete with iterators –, developed in *modern C++* (i.e. C++ $\geq$ 11).

The whole project has been conceived to be compliant with *at least* the [specifications provided](https://github.com/asartori86/advanced_programming_2019-20/blob/master/exam/readme.pdf).

After obtaining a refined working implementation, further optimizations have been carried on together with testing, to ensure code correctness, and benchmarking, to assess the performance of the final result both in a *balanced* and an *unbalanced* tree situation, and its comparison with the already existing data structures `std::map` and `std::unordered_map`, both based on self-balancing trees.

Results show that, if supported by adequate compiler optimizations, the final implementation is at least equally performing w.r.t. Intel-optimized `std::map` both in the balanced and the unbalanced case, and consistently better-performing w.r.t. GCC `std::unbalanced_map` (being an Intel-optimized version unavailable).



## Code structure and relevant design choices

The whole codebase – as well as the project development itself – has been organized into three main building blocks, which inter-operate in the global design of the template BST. Nominally: the *Binary Node*, the *Tree Iterator* (more precisely: an iterator over a structure of *binary nodes*) and the *Binary Search Tree* itself. 

As a general design rule, implementations have been initially concieved to be the most general possible – delaying eventual project-specific specializations or additions to the actual moment a different or more peculiar necessity emerged. As an example, this led to the creation of a *Binary Node* class, which is usable in both the case of a *tree composed of nodes* and that of a *generic structure composed of nodes*; likewise, the *Binary Node* class is capable of handling both generic data types – and `std::pair`s – as elements contained. In a similar fashion, the *Tree iterator* class is capable of iterating – with just minor modifications – over a generic graph composed of binary nodes.

Whenever possible, *generic programming* in the form of templating has been used in order to improve runtime performance, while also ensuring that the eventual user of such classes is able to access, through the same interface and without further modifications, any C++ data type as *key* or *value* stored in each node of the BST (or as *element of the node* more in general).

A more in-detail analysis of the classes is showed below.

### `node.hpp`: A *binary node*

A *binary node* that the class succeeded in implementing is a data structure composed of a *node* (an *empty data container*) associated to a specific data type, and eventually:

-   An *element* of such data type, which is said to be *contained* into the node;
-   At most two *child nodes*, which are said to be *connected as children* to the node;
-   At most one *parent* of the node – whithout which the node is said to be *a root*.

In the usual scenario, the *parent*-*child* relationships define the specific data structure composed of nodes, and are the only difference w.r.t. just storing data of a given datatype on their own.

It is also important to note that, being the limit on the number of parents (1) smaller than that on the number of children (2), this defines an asymmetry on the *parent-to-child* vs. *child-to-parent* axis, making it necessary for a parent (as it is the only possibility) to ensure that such limits are enforced.

As this will be useful in defining more complex, ordered data structures, a node can be thought as a *left* or *right* child (even when considered alone), if it has a parent.

On the implementation side, these properties (or invariants) have been enforced by:

-   Explicitly making impossible (through the interface) to copy nodes *as nodes*, that is without their defining relationships. In particular, deleting the copy-constructor and copy-assignment operator for a binary node still allows to copy and copy-assign the element contained (according to the specific data type specifications), but also guards against copies of *nodes as data*, requiring copy operations to be defined inside the eventual further classes making use of the nodes. This also improves performance on such derived structures, as a means of forcing copy-elision.
-   Implementing the concept of *parent-child relationship* with two unique pointers pointing from parents to children, and one `std::experimental::observer_ptr` pointing from children to the parent. That way, each parent *becomes responsible* of the children, of setting 

### `iterator.hpp`: An iterator over tree-like graph composed of binary nodes

### `bst.hpp`: A Binary Search Tree (composed of *binary nodes*)





## Testing



## Benchmarking





### APPENDIX I: How to obtain and compile the code





















bozza

describe how to compile and run code
1)valgrind
2)doxygen
3)clang

general design in three templated classes
1)to avoid code bloating(due to different template parameters) if node and iterator were implemented as bst type members
2)avoid code duplication for iterator and const iterator with conditional and templated constructor

node
1)RAII to handle memory resources. 
2)invariants: parent has ownership of node, node has ownership of children. Consequences:non symmetric relationship between parent and children, see null and detach member functions. 
3)use of smart pointers to implement RAII: unique_ptr to nodes and observer_ptr (raw ptr) to parent.
4)why shared pointers and weak pointers are not the riht choice
5)nodes are not copyable. Reasons: optimization of code in bst (copy constructor) and 
6)use of asserts for validation of pre conditions for member functions with arguments. We can vhoose to handle exceptions for invalid values or use ap_error.h as helper.
7)use of static asserts.
8)bool to represent left and right child for fast iterator moving on the bst.
9) construction of node with perfect forwarding of lvalue and rvalue references.

iterator
1)standard type alias for iterators
2)forward iterator: multiple read and write(input+output iterator), operator++ and operator++(int), dereferencing, operator ==, !=

bst
1)binary search for find and insert
2)recursion (dividi et impera) for balance, with the aid of vector to fast access contiguous memory
3)recursion for erase, O(h) where h is the deepness of the tree