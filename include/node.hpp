#pragma once
#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

template<typename T>
class Node
{
    public:
    using value_type = T;

    Node() = default;
    Node(Node&) = delete;
    Node(Node&&);
    Node(const Node&&);

    Node& operator=(const Node&) = delete;
    Node& operator=(const Node) = delete;
    Node& operator=(Node&&);
    Node& operator=(const Node&&);

    ~Node() = default;

    const T& read_elem() const;
    const std::unique_ptr<Node>& read_lc() const;
    const std::unique_ptr<Node>& read_rc() const;
    const std::shared_ptr<Node>& read_pr() const;

    template<typename ST>
    void set_lc(Node<ST>&);

    template<typename ST>
    void set_rc(Node<ST>&);

    template<typename STL, typename STR>
    void set_both_children(Node<STL>&, Node<STR>&);

    template<typename FWR>
    void write_elem(FWR&&);

    void null_left();
    void null_right();

    void info()
    {
        std::cout << "~~ A NODE ~~"
                  << "\n"
                  << "Contained element: " << this->read_elem() << "\n"
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
        std::cout << "\n\n";
    };


    private:
    T elem;

    std::unique_ptr<Node> left_child;
    std::unique_ptr<Node> right_child;
    std::shared_ptr<Node> parent;

    void null_parent();  // Must stay private to avoid disasters
};

template<typename T>
Node<T>::Node(Node&& node) :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)} {};

template<typename T>
Node<T>::Node(const Node&& node) :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)} {};


template<typename T>
Node<T>& Node<T>::operator=(Node<T>&& node)
{
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    return *this;
};

template<typename T>
Node<T>& Node<T>::operator=(const Node<T>&& node)
{
    this->elem = std::move(node.elem);
    this->left_child = std::move(node.left_child);
    this->right_child = std::move(node.right_child);
    this->parent = std::move(node.parent);
    return *this;
};

// Read-only access...
template<typename T>
const T& Node<T>::read_elem() const
{
    return this->elem;
};

template<typename T>
const std::unique_ptr<Node<T>>& Node<T>::read_lc() const
{
    return this->left_child;
};

template<typename T>
const std::unique_ptr<Node<T>>& Node<T>::read_rc() const
{
    return this->right_child;
};

template<typename T>
const std::shared_ptr<Node<T>>& Node<T>::read_pr() const
{
    return this->parent;
};

template<typename T>
template<typename FWR>
void Node<T>::write_elem(FWR&& given)
{
    this->elem = std::forward<FWR>(given);
};

// Ptr setters
template<typename T>
template<typename ST>
void Node<T>::set_lc(Node<ST>& given)
{
    given.parent.reset(this);
    this->left_child = std::make_unique<Node<ST>>(std::move(given));
};

template<typename T>
template<typename ST>
void Node<T>::set_rc(Node<ST>& given)
{
    given.parent.reset(this);
    this->right_child = std::make_unique<Node<ST>>(std::move(given));
};

template<typename T>
template<typename STL, typename STR>
void Node<T>::set_both_children(Node<STL>& l_given, Node<STR>& r_given)
{
    this->set_lc(l_given);
    this->set_rc(r_given);
};

// Utility
template<typename T>
void Node<T>::null_left()
{
    if (this->left_child != nullptr)
    {
        this->left_child.get()->null_parent();
        this->left_child = nullptr;
    }
}

template<typename T>
void Node<T>::null_right()
{
    if (this->right_child != nullptr)
    {
        this->right_child.get()->null_parent();
        this->right_child = nullptr;
    }
}

template<typename T>
void Node<T>::null_parent()
{
    this->parent = nullptr;
}


// NON-MEMBER IMPLEMENTATION
// ?
