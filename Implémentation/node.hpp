#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <vector>

class Node
{
private:
	int tab[2]; //Ne sert à rien pour le moment. Un vecteur de formule CTL sera surement utilisé.
	Node* m_father;
	std::vector<Node*> m_son;
public:
	Node(int, int, Node*);
	Node(const Node&) = default;
	~Node() = default;
	int getTab(int);
	inline Node* get_father()const{return this->m_father;}
	inline std::vector<Node*> get_son()const{return this->m_son;}
	void addSon(Node*);
};


#endif
