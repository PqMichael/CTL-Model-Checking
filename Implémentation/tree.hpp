#ifndef _TREE_HPP_
#define _TREE_HPP_
#include "node.hpp"

class Tree{
private:
	Node* m_root;
public:
	Tree();
	Tree(const Tree&) = default;
	~Tree() = default;
	void addNode(int, State, bool, bool);
private:
	Node* findNode(Node*, int);
};

#endif