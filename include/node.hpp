// DEFINES
#define DIAG

// INCLUDES
#include <cassert>
#include <memory>
#include <utility>

#if defined(DIAG)
    #include <iostream>
#endif

// CLASS
template<typename T>
class Node
{
    public:
    using value_type = T;

    /*
     * CTORS
     */

    // Default
    Node();

    // Custom (full signature, by-value)
    Node(std::unique_ptr<Node>&, std::unique_ptr<Node>&, std::unique_ptr<Node>&);

    // Copy (by-ref)
    Node(Node&) = delete;

    // Move (non-cqual)
    Node(Node&&);

    // Move (cqual)
    Node(const Node&&);

    /*
     * ASSTS
     */

    // Copy (by-ref)
    Node& operator=(const Node&) = delete;

    // Copy (by-val)
    Node& operator=(const Node) = delete;

    // Move (non-cqual)
    Node& operator=(Node&&);

    // Move (cqual)
    Node& operator=(const Node&&);

    /*
     * DTOR
     */

    ~Node();

    /*
     * GETTERS / SETTERS
     */
    // RO Getter
    const T& read_elem() const;

    // Forwarding setter
    template<typename FWR>
    void write_elem(FWR&&);


    private:
    T elem{};
    std::unique_ptr<Node> left_child = nullptr;
    std::unique_ptr<Node> right_child = nullptr;
    std::unique_ptr<Node> parent = nullptr;
};

// MEMBER IMPLEMENTATION
/*
 * CTORS
 */

// Default
template<typename T>
Node<T>::Node() : elem{}, left_child{nullptr}, right_child{nullptr}, parent{nullptr}
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Default Constructor!"
              << "\n"
              << std::endl;
#endif
};

// Custom (full signature, by-value)
template<typename T>
Node<T>::Node(std::unique_ptr<Node>& lc, std::unique_ptr<Node>& rc, std::unique_ptr<Node>& pr) :
    //left_child{std::move(lc)}, right_child{std::move(rc)}, parent{std::move(pr)}, elem{}
    elem{}

{
    // Asserts
    assert(((lc != rc) || (lc == nullptr && rc == nullptr))
           && "Left and Right children cannot be the same, unless null pointers.");
    assert(((lc != pr && lc != pr) || (lc == nullptr && rc == nullptr && pr == nullptr))
           && "Left and/or Right children cannot be the same as the parent, unless all are null pointers.");

    // Manage pointers
    //lc.swap(left_child);
    //rc.swap(right_child);
    //pr.swap(parent);
    lc == nullptr ? left_child = nullptr : left_child.swap(lc);
    rc == nullptr ? right_child = nullptr : right_child.swap(rc);
    pr == nullptr ? parent = nullptr : parent.swap(pr);

    // Diagnostics
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Custom (FS) Constructor!"
              << "\n"
              << std::endl;
#endif
};

// Copy (by-ref) <-| DELETED
// template<typename T>
// Node<T>::Node(Node&){};

// Move (non-cqual)
template<typename T>
Node<T>::Node(Node&& node) :
    elem{std::move}, left_child{std::move(node.left_child)}, right_child{std::move(node.right_child)}, parent{std::move(
                                                                                                         node.parent)}
{
    // Diagnostics
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Move Constructor (NON-CONST)!"
              << "\n"
              << std::endl;
#endif
};

// Move (cqual)
template<typename T>
Node<T>::Node(const Node&& node) :
    elem{std::move}, left_child{std::move(node.left_child)}, right_child{std::move(node.right_child)}, parent{std::move(
                                                                                                         node.parent)}
{
    // Diagnostics
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Move Constructor (CONST)!"
              << "\n"
              << std::endl;
#endif
};

/*
 * ASSTS
 */

// Move (non-cqual)
template<typename T>
Node<T>& Node<T>::operator=(Node<T>&& node)
{
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    // Diagnostics
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Move Assignment (NON-CONST)!"
              << "\n"
              << std::endl;
#endif
    return *this;
};

// Move (cqual)
template<typename T>
Node<T>& Node<T>::operator=(const Node<T>&& node)
{
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    // Diagnostics
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Move Assignment (CONST)!"
              << "\n"
              << std::endl;
#endif
    return *this;
};

/*
* DTOR
*/
template<typename T>
Node<T>::~Node()
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Destructor!"
              << "\n"
              << std::endl;
#endif
};

/*
* GETTERS / SETTERS
*/
template<typename T>
const T& Node<T>::read_elem() const
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Getting: elem (ROR)!"
              << "\n"
              << std::endl;
#endif
    return elem;
};

template<typename T>
template<typename FWR>
void Node<T>::write_elem(FWR&& given)
{
    elem = std::forward<FWR>(given);  // Copy? Move? Not our problem anymore... ¯\_(ツ)_/¯
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Setting: elem (FROM FORWARDING)!"
              << "\n"
              << std::endl;
#endif
};

// NON-MEMBER IMPLEMENTATION
// ?
