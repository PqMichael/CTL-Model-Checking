#include "node.hpp"
#include <iostream>

extern int N_NODE;

Node::Node(State etat, bool a, bool b, Node* father){
	std::cout<<"Création du noeud n° "<<N_NODE<<std::endl;
	this->tab[0] = a;
	this->tab[1] = b;
	this->m_father = father;
	if (this->m_father != nullptr){this->m_father->addSon(this);}
	this->m_son.push_back(nullptr);
	this->s = etat;
	this->ID = N_NODE;
	N_NODE++;
}

void Node::addSon(Node* son){
	if (this->m_son[0] == nullptr){m_son.erase(m_son.begin());}
	this->m_son.push_back(son);
}

void Node::set_state(int indice, bool state){
	this->tab[indice] = state;
}

