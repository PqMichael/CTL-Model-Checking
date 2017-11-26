#include "node.hpp"
#include <iostream>

Node::Node(int a, int b, Node* father){
	this->tab[0] = a;
	this->tab[1] = b;
	this->m_father = father;
	if (this->m_father != nullptr){this->m_father->addSon(this);}
	this->m_son.push_back(nullptr);
}

int Node::getTab(int indice){
	return tab[indice];
}

void Node::addSon(Node* son){
	if (this->m_son[0] == nullptr){m_son.erase(m_son.begin());}
	this->m_son.push_back(son);
}

int main(){
	std::cout<<"coucou"<<std::endl;
	Node x = Node(5, 6, nullptr);
	std::cout<<x.get_son()[0]<<std::endl;
	Node y = Node(1, 2, &x);
	std::cout<<x.get_son()[0]<<std::endl;
	std::cout<<y.get_father()<<std::endl;
	return 0;
}