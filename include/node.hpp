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

#include <cassert>              // Safety tests <- TO BE IMPLEMENTED
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
    inline Node(FWR&&) noexcept;

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

    inline void set_lc(Node<T>*&);
    inline void set_rc(Node<T>*&);
    inline void set_both_children(Node<T>*&, Node<T>*&);

    template<typename FWR>
    inline void write_elem(FWR&&);

    inline void null_left() noexcept;
    inline void null_right() noexcept;

#if defined(DIAG)
    void info() const noexcept;  // If you wanted this inline, you're using it wrong ;)
    friend std::ostream& operator<<(std::ostream& os, const Node& node){
      os << "(" << node.read_elem().first
         << "," << node.read_elem().second << ")";
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
    inline void set_parent(Node<T>*);
};

template<typename T>
template<typename FWR>
inline Node<T>::Node(FWR&& given) noexcept : elem{std::forward<FWR>(given)} {};

template<typename T>
inline Node<T>::Node(Node&& node) noexcept :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)} {};


template<typename T>
inline Node<T>& Node<T>::operator=(Node<T>&& node) noexcept
{
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
    return this->elem;
};

template<typename T>
inline const std::unique_ptr<Node<T>>& Node<T>::read_lc() const noexcept
{
    return this->left_child;
};

template<typename T>
inline const std::unique_ptr<Node<T>>& Node<T>::read_rc() const noexcept
{
    return this->right_child;
};

template<typename T>
inline const std::experimental::observer_ptr<Node<T>>& Node<T>::read_pr() const noexcept
{
    return this->parent;
};

template<typename T>
template<typename FWR>
inline void Node<T>::write_elem(FWR&& given)
{
    this->elem = std::forward<FWR>(given);
};

// Ptr setters
template<typename T>
inline void Node<T>::set_lc(Node<T>*& given)
{
    assert(this->left_child == nullptr);
    this->left_child.reset(given);
    this->left_child.get()->set_parent(this);
    this->left_child.get()->set_childtype(false);
};

template<typename T>
inline void Node<T>::set_rc(Node<T>*& given)
{
    assert(this->right_child == nullptr);
    this->right_child.reset(given);
    this->right_child.get()->set_parent(this);
    this->right_child.get()->set_childtype(true);
};

template<typename T>
inline void Node<T>::set_both_children(Node<T>*& l_given, Node<T>*& r_given)
{
    this->set_lc(l_given);
    this->set_rc(r_given);
};

// Utility
template<typename T>
inline void Node<T>::null_left() noexcept
{
    if (this->left_child != nullptr)
    {
        this->left_child.get()->null_parent();
        this->left_child.release();
        this->left_child = nullptr;
    }
}

template<typename T>
inline void Node<T>::null_right() noexcept
{
    if (this->right_child != nullptr)
    {
        this->right_child.get()->null_parent();
        this->right_child.release();
        this->right_child = nullptr;
    }
}

template<typename T>
inline void Node<T>::null_parent() noexcept
{
    this->parent.reset();
}

template<typename T>
inline void Node<T>::set_parent(Node<T>* newparent)
{
    this->parent.reset(newparent);
}

template<typename T>
inline void Node<T>::set_childtype(bool childbool) noexcept
{
    this->childtype = childbool;
}

template<typename T>
inline bool Node<T>::is_left() noexcept
{
    return !this->childtype;
}

template<typename T>
inline bool Node<T>::is_right() noexcept
{
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
