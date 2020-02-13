/******************************************************************************\
 * |> NODE a.k.a. node.hpp <|                                                 *
 *                                                                            *
 * "That's my job here: crafting high-performance nodes"                      *
 *                                                                            *
 *                                         (an author, on this header file)   *
 *                                                                            *
\******************************************************************************/

#pragma once
#define DIAG  // Remove/comment this before "shipping to production"

#include <cassert>              // Safety tests
#include <experimental/memory>  // Use: observer_ptr | we REALLY want a raw ptr with the same semantics as smart ptr
#include <memory>               // Use: unique_ptr

#if defined(DIAG)
    #include <iostream>  // Use: cout, endl
#endif

template<typename T>
class Node
{
    public:
    using elem_type = T;

    inline Node() noexcept = default;

    template<typename FWR>
    inline explicit Node(FWR&&) noexcept;  // We're very lucky it doesn't happen ;)

    Node(Node&) = delete;  // Explicit deletion as in GNU unique_ptr.h
    inline Node(Node&&) noexcept;

    Node& operator=(const Node&) = delete;  // Explicit deletion as in GNU unique_ptr.h
    inline Node& operator=(Node&&) noexcept;

    inline ~Node() noexcept = default;

    inline const T& read_elem() const noexcept;
    inline const std::unique_ptr<Node<T>>& read_lc() const noexcept;
    inline const std::unique_ptr<Node<T>>& read_rc() const noexcept;
    inline const std::experimental::observer_ptr<Node<T>>& read_pr() const noexcept;
    inline bool is_left() noexcept;
    inline bool is_right() noexcept;

    inline void set_lc(Node<T>*);
    inline void set_rc(Node<T>*);
    inline void set_children(Node<T>*, Node<T>*) noexcept;

    template<typename FWR>
    inline void write_elem(FWR&&);

    inline void detach_left() noexcept;
    inline void detach_right() noexcept;
    inline void detach_children() noexcept;

    inline void null_left() noexcept;
    inline void null_right() noexcept;
    inline void null_children() noexcept;

    //add detach
#if defined(DIAG)
    void info() const noexcept;  // If you wanted this inline, you're using it wrong ;)
    friend std::ostream& operator<<(std::ostream& os, const Node& node)
    {
        os << "(" << node.read_elem().first << "," << node.read_elem().second << ")";
        return os;
    }
#endif


    private:
    T elem;

    std::unique_ptr<Node<T>> left_child;
    std::unique_ptr<Node<T>> right_child;
    std::experimental::observer_ptr<Node<T>> parent;

    bool childtype{false};  // left = false, right = true
    inline void set_childtype(bool) noexcept;

    inline void null_parent() noexcept;
    inline void set_parent(Node<T>*) noexcept;
};

template<typename T>
template<typename FWR>
inline Node<T>::Node(FWR&& given) noexcept : elem{std::forward<FWR>(given)}
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: FWDing ctor of Element!"
              << "\n"
              << std::endl;
#endif
};  // We're very lucky it doesn't happen ;)

template<typename T>
inline Node<T>::Node(Node&& node) noexcept :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)}
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Move ctor"
              << "\n"
              << std::endl;
#endif
};


template<typename T>
inline Node<T>& Node<T>::operator=(Node<T>&& node) noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Mvasst!"
              << "\n"
              << std::endl;
#endif
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    return *this;
};

// Read-only access...
template<typename T>
inline const T& Node<T>::read_elem() const noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Read Element!"
              << "\n"
              << std::endl;
#endif
    return this->elem;
};

template<typename T>
inline const std::unique_ptr<Node<T>>& Node<T>::read_lc() const noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Read LC (uniqueptr)!"
              << "\n"
              << std::endl;
#endif
    return this->left_child;
};

template<typename T>
inline const std::unique_ptr<Node<T>>& Node<T>::read_rc() const noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Read RC (uniqueptr)!"
              << "\n"
              << std::endl;
#endif
    return this->right_child;
};

template<typename T>
inline const std::experimental::observer_ptr<Node<T>>& Node<T>::read_pr() const noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Read PR (Observer ptr)!"
              << "\n"
              << std::endl;
#endif
    return this->parent;
};

template<typename T>
template<typename FWR>
inline void Node<T>::write_elem(FWR&& given)
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: FWDing write Element!"
              << "\n"
              << std::endl;
#endif
    this->elem = std::forward<FWR>(given);
};

// Ptr setters
template<typename T>
inline void Node<T>::set_lc(Node<T>* given)
{
    assert(given);                      // Can't set to nullptr this way; use null_left() instead
    assert(!(this->left_child.get()));  // Can set only if free
    assert(!(this->parent.get()) || this->parent.get() != given);  // Can't create cycles

#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Set LC (from NAKED PTR)!"
              << "\n"
              << std::endl;
#endif

    this->left_child.reset(given);
    this->left_child.get()->set_parent(this);
    this->left_child.get()->set_childtype(false);
};

template<typename T>
inline void Node<T>::set_rc(Node<T>* given)
{
    assert(given);                       // Can't set to nullptr this way; use null_right() instead
    assert(!(this->right_child.get()));  // Can set only if free
    assert(!(this->parent.get()) || this->parent.get() != given);  // Can't create cycles

#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Set RC (from NAKED PTR)!"
              << "\n"
              << std::endl;
#endif

    this->right_child.reset(given);
    this->right_child.get()->set_parent(this);
    this->right_child.get()->set_childtype(true);
};

template<typename T>
inline void Node<T>::set_children(Node<T>* l_given, Node<T>* r_given) noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Set BOTH CHILDREN (double call)!"
              << "\n"
              << std::endl;
#endif

    this->set_lc(l_given);
    this->set_rc(r_given);
}

template<typename T>
inline void Node<T>::detach_children() noexcept
{

#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Detach BOTH CHILDREN!"
              << "\n"
              << std::endl;
#endif
    this->detach_left();
    this->detach_right();
}

template<typename T>
inline void Node<T>::null_children() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Null BOTH CHILDREN!"
              << "\n"
              << std::endl;
#endif
    this->null_left();
    this->null_right();
};

// Utility
template<typename T>
inline void Node<T>::detach_left() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Detach LC!"
              << "\n"
              << std::endl;
#endif


    if (this->left_child)
    {
        this->left_child.get()->null_parent();
        this->left_child.release();
        this->left_child = nullptr;
    }
}

template<typename T>
inline void Node<T>::detach_right() noexcept
{

#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Detach RC!"
              << "\n"
              << std::endl;
#endif
    if (this->right_child)
    {
        this->right_child.get()->null_parent();
        this->right_child.release();
        this->right_child = nullptr;
    }
}

template<typename T>
inline void Node<T>::null_left() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Null LC!"
              << "\n"
              << std::endl;
#endif
    if (this->left_child)
    {
        this->left_child.reset();
    }
}

template<typename T>
inline void Node<T>::null_right() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Null RC!"
              << "\n"
              << std::endl;
#endif
    if (this->right_child)
    {
        this->right_child.reset();
    }
}

template<typename T>
inline void Node<T>::null_parent() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Null parent!"
              << "\n"
              << std::endl;
#endif
    this->parent.reset();
}

template<typename T>
inline void Node<T>::set_parent(Node<T>* newparent) noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Set parent!"
              << "\n"
              << std::endl;
#endif

    this->parent.reset(newparent);
}

template<typename T>
inline void Node<T>::set_childtype(bool childbool) noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: Set Childtype"
              << "\n"
              << std::endl;
#endif
    this->childtype = childbool;
}

template<typename T>
inline bool Node<T>::is_left() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called: IsLeft?!"
              << "\n"
              << std::endl;
#endif
    return !this->childtype;
}

template<typename T>
inline bool Node<T>::is_right() noexcept
{
#if defined(DIAG)
    std::cout << "[DIAG]: "
              << "Called:IsRight?!"
              << "\n"
              << std::endl;
#endif
    return this->childtype;
}

#if defined(DIAG)
template<typename T>
void Node<T>::info() const noexcept
{
    std::cout << "~~ A NODE ~~"
              << "\n"
              << "Contained element: " << *this << "\n"
              << "Left child: ";
    if (this->left_child == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_lc().get();
    }
    std::cout << "\n";
    std::cout << "Right child: ";
    if (this->right_child == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_rc().get();
    }
    std::cout << "\n";
    std::cout << "Parent: ";
    if (this->parent == nullptr)
    {
        std::cout << "NONE";
    }
    else
    {
        std::cout << this->read_pr().get();
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
