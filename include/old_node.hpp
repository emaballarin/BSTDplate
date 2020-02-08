#include <memory>
#include <utility>

template<typename T>
class Node
{
    public:
    using value_type = T;

    /*
    * CONSTRUCTORS
    */

    Node() = default;
    Node(std::unique_ptr<Node> lc, std::unique_ptr<Node> rc, std::shared_ptr<Node> pt);  //implement asserts

    // Constructors have always 'the class they construct' as return type... | Copyee must be const-qualified
    Node(const Node&) = delete;

    // Constructors have always 'the class they construct' as return type...
    Node(Node&& node);
    Node(const Node&& node);  // Only useful if we want to build nodes from FUNCTIONS that return 'const Node&&'
                              // Maybe remove?

    /*
    * DESTRUCTORS
    */

    ~Node() = default;  // ENOUGH??

    /*
    * ASSIGNMENTS
    */

    // If the LHS is const, we can just construct; never assign.
    Node& operator=(const Node&) = delete;

    // We have 4 to choose from only 2 (cannot overload on ret-type alone)
    // If we const-qualify LHS, what does it mean to assign to?
    Node& operator=(Node&& node)
    {
        elem = std::move(node.elem);
        left_child = std::move(node.left_child);
        right_child = std::move(node.right_child);
        parent = std::move(node.parent);
        return *this;
    };
    //const Node& operator=(Node&& node);
    Node& operator=(const Node&& node)  // Exact copy of the above...
    {
        elem = std::move(node.elem);
        left_child = std::move(node.left_child);
        right_child = std::move(node.right_child);
        parent = std::move(node.parent);
        return *this;
    };  // Only useful if we want to build nodes from FUNCTIONS that return 'const Node&&'
      // Maybe remove?
    //const Node& operator=(const Node&& node);

    //    const T get_elem()  // What to return?
    //    {
    //        return elem;
    //    }

    private:
    T elem;  // Beware of the case in which T is a native type |-> garbage values
    std::unique_ptr<Node> left_child;
    std::unique_ptr<Node> right_child;
    std::shared_ptr<Node> parent;
};

template<typename T>
Node<T>::Node(std::unique_ptr<Node> lc, std::unique_ptr<Node> rc, std::shared_ptr<Node> pt) :
    left_child{lc}, right_child{rc}, parent{pt}
{
    static_assert(((lc != rc) || (lc == nullptr && rc == nullptr))
                  && "Left and Right children cannot be the same, unless null pointers.");
    static_assert(((lc != pt && lc != pt) || (lc == nullptr && rc == nullptr && pt == nullptr))
                  && "Left and/or Right children cannot be the same as the parent, unless all are null pointers.");
}


template<typename T>
Node<T>::Node(Node&& node) :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)}
{
}

// An exact copy of that above...
template<typename T>
Node<T>::Node(const Node&& node) :
    elem{std::move(node.elem)}, left_child{std::move(node.left_child)},
    right_child{std::move(node.right_child)}, parent{std::move(node.parent)}
{
}
