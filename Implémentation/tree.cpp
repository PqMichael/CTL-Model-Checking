#include "tree.hpp"
#include "node.hpp"
#include "kripke.hpp"
#include <iostream>
#include <string>

extern int N_NODE;
int SIZEOFTREE = 3;

Tree::Tree(Kripke* K){
	std::string state = K->getS0(0);
	std::vector<std::string> prop = K->getI(state);
	m_root = new Node(state, prop, nullptr);
	buildTree(K, 0, m_root);
}

void Tree::buildTree(Kripke* K, int step, Node* node){
	std::vector<std::string> states = K->getR(node->get_state());
	std::vector<std::string>::iterator it;
	for(it = states.begin(); it != states.end(); it++){
		Node* result = addNode(node->get_ID(), *it, K->getI(*it));
		if (step < SIZEOFTREE){buildTree(K, step+1, result);}
	}
}

Node *Tree::addNode(int father_ID, std::string s, std::vector<std::string> prop){
	//Cherche le noeud auquel attacher le nouveau, et l'attache à l'arbre
	Node* father = findNode(this->m_root, father_ID);
	if (father == nullptr){
		std::cout<<"Le demande était de créer le noeud attaché à l'ID "<<father_ID<<std::endl;
		std::cout<<"ID non trouvé, noeud supprimé"<<std::endl;
		return nullptr;
	}
	else {return new Node(s, prop, father);}
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

