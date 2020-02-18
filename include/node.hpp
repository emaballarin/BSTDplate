/******************************************************************************\
 * |> NODE a.k.a. node.hpp <|                                                 *
 *                                                                            *
 * "That's my job here: crafting high-performance nodes"                      *
 *                                                                            *
 *                                         (an author, on this header file)   *
 *                                                                            *
\******************************************************************************/

/**
 * @file node.hpp
 * @author Leonardo Arrighi
 * @author Francesco Romor
 * @author Emanuele Ballarin
 * @date 18 February 2020
 * @brief Header file containing the implementation of a type-templated binary node, whith auxiliary functions.
 */

#pragma once  // Easy ODR
//#define DIAG  // Remove/comment this before "shipping to production" (e.g. to disable asserts)

#include <cassert>              // Safety tests
#include <experimental/memory>  // Use: observer_ptr | we really want a raw ptr with the same semantics as smart ptr
#include <memory>               // Use: unique_ptr

#if defined(DIAG)
    #include <iostream>  // Output to STDOUT
#endif

/**
 * @brief Type-templated binary node
 * @tparam T Typename of the element belonging to a node
 */
template<typename T>
class Node
{
    public:
    using elem_type = T;

    /**
     * @brief Default constructor for a type-templated node
     *
     * Calls the default constructor on all private variables
     */
    inline Node() noexcept = default;

    /**
     * @brief Custom constructor for a type-templated node, with perfect forwarding of the element to be contained in the node
     * @tparam FWR Typename of the element belonging to a type-tamplated node
     * @param given The node-element-likely-typed object to be set as the new-node element
     *
     * It is not possible to use it to construct nodes containing a node.
     */
    template<typename FWR>
    explicit Node(FWR&& given) noexcept : elem{std::forward<FWR>(given)} {};  // Acceptable warning (clang-tidy)

    /**
     * @brief Copy constructor for a type-templated node
     *
     * Deleted for performance reasons and on philosophical grounds: what does it mean to copy a node without its mutual relationships?
     */
    Node(Node&) = delete;  // Explicit deletion as in GNU unique_ptr.h

    /**
     * @brief Move constructor for a type-templated node
     * @param node The node to be moved
     */
    Node(Node&& node) noexcept :
        elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
        right_child{std::move(node.right_child)}, parent{std::move(node.parent)} {};

    /**
     * @brief Copy assignment for a type-templated node
     *
     * Deleted for performance reasons and on philosophical grounds: what does it mean to copy a node without its mutual relationships?
     */
    Node& operator=(const Node&) = delete;  // Explicit deletion as in GNU unique_ptr.h

    Node& operator=(Node&&) noexcept;

    /**
     * @brief Getter for the element contained into a type-templated node
     * @return Const-qualified reference to the element contained into the node
     */
    const T& read_elem() const noexcept
    {
        return this->elem;
    };

    /**
     * @brief Getter for the std::unique_ptr pointing to the node constituting the left child
     * @return Const-qualified reference to the std::unique_ptr pointing to the node constituting the left child
     */
    const std::unique_ptr<Node<T>>& read_lc() const noexcept
    {
        return this->left_child;
    };

    /**
     * @brief Getter for the std::unique_ptr pointing to the node constituting the right child
     * @return Const-qualified reference to the std::unique_ptr pointing to the node constituting the right child
     */
    const std::unique_ptr<Node<T>>& read_rc() const noexcept
    {
        return this->right_child;
    };

    /**
     * @brief Getter for the std::experimental::unique_ptr pointing to the node constituting the parent
     * @return Const-qualified reference to the std::experimental::unique_ptr pointing to the node constituting the parent
     *
     * The use of the experimental "world's dumbest smart pointer" (a.k.a. observer pointer) ensures a consistent semantic among smart pointers, without implementing the concept of ownership or uniqueness. If a node is a root (parentless node), it points to nullptr
     */
    const std::experimental::observer_ptr<Node<T>>& read_pr() const noexcept
    {
        return this->parent;
    };

    /**
     * @brief Default destructor for a type-templated node
     *
     * Calls the default destructor on all members
     */
    inline ~Node() noexcept = default;

    template<typename FWR>
    inline void write_elem(FWR&&);

    void set_lc(Node<T>*);
    void set_rc(Node<T>*);

    template<typename XK, typename XV>
    inline XV& value_refrw();

    void detach_left() noexcept;
    void detach_right() noexcept;

    /**
     * @brief Setter for both the std::unique_ptr pointing to the children
     * @param l_given Raw pointer to the node you want to assign as the left child. Can be used with operator new
     * @param r_given Raw pointer to the node you want to assign as the right child. Can be used with operator new
     */
    void set_children(Node<T>* l_given, Node<T>* r_given)
    {
        this->set_lc(l_given);
        this->set_rc(r_given);
    }

    /**
     * @brief Releases the std::unique_ptr pointing to both the children, making them roots.
     *
     * The function called on a node having a nullptr as children (or any of them), does not throw
     */
    void detach_children() noexcept
    {
        this->detach_left();
        this->detach_right();
    }

    /**
     * @brief Setter to null for both the std::unique_ptr pointing to both the left and right child, triggering memory release
     *
     * The function called on a node having a nullptr as children (or any of them), does not throw
     */
    void null_children() noexcept
    {
        this->null_left();
        this->null_right();
    };
    /**
     * @brief Setter to null for the std::unique_ptr pointing to the left child, triggering memory release
     * The function called on a node having a nullptr as child, does not throw
     */
    void null_left() noexcept
    {
        if (this->left_child)
        {
            this->left_child.reset();
        }
    }

    /**
     * @brief Setter to null for the std::unique_ptr pointing to the right child, triggering memory release
     *
     * The function called on a node having a nullptr as child, does not throw
     */
    void null_right() noexcept
    {
        if (this->right_child)
        {
            this->right_child.reset();
        }
    }

    /**
     * @brief Checks if the node is a left child
     * @return Boolean value answering to the question if the node is a left child
     *
     * By default, for implementation efficiency reasons, a root is a left child. Discrimination among left children and roots can happen by checking explicitly the value of the parent pointer
     */
    bool is_left() const noexcept
    {
        return !(this->childtype);
    }

    /**
     * @brief Checks if the node is a right child
     * @return Boolean value answering to the question if the node is a right child
     */
    bool is_right() const noexcept
    {
        return this->childtype;
    }

    //add detach
#if defined(DIAG)
    void info() const noexcept;  // If you wanted this inline, you're using it wrong ;)
#endif


    private:
    /** The element belonging to a node */
    T elem;

    /** The std::unique_ptr pointing to the node that is the left child */
    std::unique_ptr<Node<T>> left_child;

    /** The std::unique_ptr pointing to the node that is the right child */
    std::unique_ptr<Node<T>> right_child;

    /** The std::experimental::observer_ptr that points to the node that is the parent */
    std::experimental::observer_ptr<Node<T>> parent;

    /** Right-/left- - ness of node-childship as a boolean; left is represented by the false value; a root is false by default */
    bool childtype{false};  // left = false, right = true; by default the root is a left child

    /**
     * @brief Setter of the std::experimental::observer_ptr pointing to the parent, by reset
     * @param newparent The raw pointer to the node to be set as the parent
     */
    void set_parent(Node<T>* newparent) noexcept
    {
        this->parent.reset(newparent);
    }

    /**
     * @brief Setter of right-/left- -ness of a node upon parent selection
     * @param newparent The node to be set as the new node
     */
    void set_childtype(bool childbool) noexcept
    {
        this->childtype = childbool;
    }

    /**
     * @brief Setter to null for the std::experimental::observer_ptr pointing to the parent, by reset
     */
    void null_parent() noexcept
    {
        this->parent.reset();
        this->childtype = false;
    }
};


/**
 * @brief Move assignment for a type-templated node
 * @tparam T Typename of the element belonging to a node
 * @param node The node to be move-assigned
 */
template<typename T>
inline Node<T>& Node<T>::operator=(Node<T>&& node) noexcept
{
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    return *this;
};

/**
 * @brief Setter for the element contained into a type-templated node, with perfect forwarding of the element passed
 * @tparam T Typename of the element belonging to a node
 * @tparam FWR Typename of the element belonging to a type-tamplated node
 * @param given The element you want the node to contain, perfectly-forwarded
 *
 * It is not possible to use it to assign nodes as elements of other nodes.
 */
template<typename T>
template<typename FWR>
inline void Node<T>::write_elem(FWR&& given)
{
    static_assert(std::is_same<T, std::remove_reference<FWR>>::value == true);
    this->elem = std::forward<FWR>(given);
};

/**
 * @brief Getter for a reference (eventually writable) to the value of the element of the node, if the node is std::pair-typed
 * @tparam T Typename of the element belonging to a node
 * @tparam XK Typename of the key of the element of the node, if the node is std::pair-typed
 * @tparam XV Typename of the value of the element of the node, if the node is std::pair-typed
 * @return Reference (eventually writable) to the value of the element of the node, if the node is std::pair-typed
 *
 * The use of nested templates ensures availability of the member function only with adequate types, while avoiding to resort to more sophisticate template idioms
 */
template<typename T>
template<typename XK, typename XV>
inline XV& Node<T>::value_refrw()
{
    static_assert(std::is_same<T, std::pair<XK, XV>>::value == true);

    return this->elem.second;
};

/**
 * @brief Setter for the std::unique_ptr pointing to the left child
 * @tparam T Typename of the element belonging to a node
 * @param given Raw pointer to the node you want to assign as the left child. Can be used with operator new
 *
 * It is not possible to set as a child the parent itself. If a nullptr is passed, the child-pointing std::unique_ptr is nulled.
 * When a child is assigned, its pointer pointing to parent is automatically set as pointing to the caller.
 * By default, for implementation efficiency reasons, a root is a left child. Discrimination among left children and roots can happen by checking explicitly the value of the parent pointer
 */
template<typename T>
void Node<T>::set_lc(Node<T>* given)
{
    assert(!(this->parent.get()) || this->parent.get() != given);  // Can't create cycles
    if (!given)
    {
        this->null_left();
    }
    else
    {
        assert(!(this->left_child.get()));  // Can set only if free
        this->left_child.reset(given);
        this->left_child.get()->set_parent(this);
        this->left_child.get()->set_childtype(false);
    }
};

/**
 * @brief Setter for the std::unique_ptr pointing to the right child
 * @tparam T Typename of the element belonging to a node
 * @param given Raw pointer to the node you want to assign as the right child. Can be used with operator new
 *
 * It is not possible to set as a child the parent itself. If a nullptr is passed, the child-pointing std::unique_ptr is nulled.
 * When a child is assigned, its pointer pointing to parent is automatically set as pointing to the caller.
 */
template<typename T>
void Node<T>::set_rc(Node<T>* given)
{
    assert(!(this->parent.get()) || this->parent.get() != given);  // Can't create cycles

    if (!given)
    {
        this->null_right();
    }
    else
    {
        assert(!(this->right_child.get()));  // Can set only if free
        this->right_child.reset(given);
        this->right_child.get()->set_parent(this);
        this->right_child.get()->set_childtype(true);
    }
};

/**
 * @brief Releases the std::unique_ptr pointing to the left child, making the child a root.
 * @tparam T Typename of the element belonging to a node
 *
 * The function called on a node having a nullptr as child, does not throw
 */
template<typename T>
inline void Node<T>::detach_left() noexcept
{
    if (this->left_child)
    {
        this->left_child.get()->null_parent();
        this->left_child.release();
        this->left_child = nullptr;
    }
}

/**
 * @brief Releases the std::unique_ptr pointing to the right child, making the child a root.
 * @tparam T Typename of the element belonging to a node
 *
 * The function called on a node having a nullptr as child, does not throw
 */
template<typename T>
inline void Node<T>::detach_right() noexcept
{
    if (this->right_child)
    {
        this->right_child.get()->null_parent();
        this->right_child.release();
        this->right_child = nullptr;
    }
}

#if defined(DIAG)
/**
 * @brief Prints diagnostic information about a node
 * @tparam T Typename of the element belonging to a node
 */
template<typename T>
void Node<T>::info() const noexcept
{
    std::cout << "~~ A NODE ~~"
              << "\n"
              << "Contained element: " << this->read_elem() << this->read_elem() << "\n"
              << "Left child: ";
    if (this->left_child == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_lc().get()->read_elem();
    }
    std::cout << "\n";
    std::cout << "Right child: ";
    if (this->right_child == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_rc().get()->read_elem();
    }
    std::cout << "\n";
    std::cout << "Parent: ";
    if (this->parent == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_pr().get()->read_elem();
    }
    std::cout << "\n";
    std::cout << "This is a ";
    if (this->childtype)
    {
        std::cout << "RIGHT ";
    }
    else
    {
        std::cout << "LEFT ";
    }
    std::cout << "child";
    if (!this->read_pr().get())
    {
        std::cout << " (but it actually doesn't matter!)";
    }
    std::cout << ".\n\n";
}
#endif
