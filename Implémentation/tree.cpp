#include "tree.hpp"
#include "node.hpp"
#include <iostream>

extern int N_NODE;

Tree::Tree(){
	m_root = new Node(un, false, false, nullptr);
}

void Tree::addNode(int father_ID, State s, bool a, bool b){
	//Cherche le noeud auquel attacher le nouveau, et l'attache à l'arbre
	Node* father = findNode(this->m_root, father_ID);
	if (father == nullptr){
		std::cout<<"Le demande était de créer le noeud attaché à l'ID "<<father_ID<<std::endl;
		std::cout<<"ID non trouvé, noeud supprimé"<<std::endl;
	}
	else {new Node(s, a, b, father);}
}

Node* Tree::findNode(Node* begin, int ID){
	//Parcours de l'arbre recursivement jusqu'a trouver l'ID voulu
	Node* temp = begin;
	if (temp->get_ID() == ID){return temp;}
	else{
		std::vector<Node*> sons = temp->get_son();
		std::vector<Node*>::iterator it;
		for(it = sons.begin(); it != sons.end(); it++){
			if (*it != nullptr){
				Node* result = findNode(*it, ID);
				if (result != nullptr){return result;}
			}
		}
	}
	return nullptr;
}