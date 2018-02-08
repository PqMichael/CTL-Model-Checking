#ifndef _TREE_HPP_
#define _TREE_HPP_
#include "node.hpp"
#include "kripke.hpp"
#include <string>

class Tree{
private:
	Node* m_root;
public:
	Tree(Kripke*);
	Tree(const Tree&) = default;
	~Tree() = default;
private:
	Node* findNode(Node*, int);
	void buildTree(Kripke*, int, Node*);
	Node* addNode(int, std::string, std::vector<std::string>);
};

#endif