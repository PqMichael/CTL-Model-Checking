#include "node.hpp"
#include <iostream>
#include <string>
#include <vector>

extern int N_NODE;

Node::Node(std::string etat, std::vector<std::string> prop, Node* father){
	std::cout<<"Création du noeud n° "<<N_NODE<<std::endl;
	std::cout<<"Père = "<<father<<" etat = "<<etat<<std::endl;
	this->prop = prop;
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


