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
5)nodes are not copyable. Reasons: optimization of code in bst (copy constructor)
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
