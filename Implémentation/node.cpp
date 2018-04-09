#include "node.hpp"
#include <iostream>
#include <string>
#include <vector>

//Correspond a l'ID du noeud, incremente a chaque creation de noeud
int N_NODE;

/*
*Constructeur de la classe Node. Lors de la creation d'un noeud, celui-ci doit etre ratache a son pere
*Son pere quand a lui doit maintenant avoir connaissance de la presence de son nouveau fils. L'ID du noeud cree
*est une valeur incrementee a chaque cration de noeud.
*
*@Param etat 			Etat du noeud cree
*@Param prop 			Propositions du noeud cree
*@Param father 			Pere du noeud cree
*/
Node::Node(std::string etat, std::vector<std::string> prop, Node* father){
	std::cout<<"Création du noeud n° "<<N_NODE<<std::endl;
	if (father == nullptr){
		std::cout<<"Père = Pas de père "<<" etat = "<<etat<<std::endl;
	}
	else{
		std::cout<<"Père = "<<father->get_ID()<<" etat = "<<etat<<std::endl;
	}
	this->prop = prop;
	this->m_father = father;
	if (this->m_father != nullptr) {
		this->m_father->addSon(this);
	}
	this->m_son.push_back(nullptr);
	this->s = etat;
	this->ID = N_NODE;
	N_NODE++;
}

/*
*Ajoute un fils a un noeud pere.
*
*@Param son 			Fils a ajouter
*/
void Node::addSon(Node* son){
	if (this->m_son[0] == nullptr) {
		m_son.erase(m_son.begin());
	}
	this->m_son.push_back(son);
}

/*
*Verifie la presence d'un label voulu dans les propositions du noeud.
*
*@Param formula 		Label voulu
*@Return 				Renvoie true si le label voulu est present, false sinon
*/
bool Node::isFormulaPresent(std::string formula){
	std::vector<std::string>::iterator it;
	bool isPresent = false;
	for(it = this->prop.begin(); it != this->prop.end(); it++){
		if (*it == formula) {
			isPresent = true;
		}
	}
	return isPresent;
}

/*
*Ajoute le label voulu aux propositions du noeud.
*
*@Param formula 		Label voulu
*/
void Node::addLabel(std::string formula){
	this->prop.push_back(formula);
}

/*
*Supprime le label voulu aux propositions du noeud.
*
*@Param formula 		Label voulu
*/
void Node::removeLabel(std::string formula){
	std::vector<std::string>::iterator it;
	for(it = this->prop.begin(); it != this->prop.end();){
		if(*it == formula){
			this->prop.erase(it);
		}
		else{
			it++;
		}
	}
}

