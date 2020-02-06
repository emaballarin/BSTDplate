#include <memory>
#include <utility>

template<typename T>
class Node{
public:
	using value_type = T;
	
	Node()=default;//?change default values
	Node(std::unique_ptr<Node> lc, std::unique_ptr<Node> rc, std::shared_ptr<Node> pa);//implement asserts
	
	Node& Node(const Node&);
	const Node& Node(const Node&) const;

	Node& Node(Node&& node);//?move unique_ptr
	const Node& Node(const Node&& node);//?move unique_ptr

	~Node();

	Node& operator=(const Node&);
	const Node& operator=(const Node&);

	Node& operator=(Node&& node);
	const Node& operator=(const Node&& node);
	
private:
	T elem;
	std::unique_ptr<Node> left_child;
	std::unique_ptr<Node> right_child;
	std::shared_ptr<Node> parent;//?default initialization shared_pointer

}

