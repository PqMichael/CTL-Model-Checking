#include "kripke.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>

bool HASCHANGEDDURINGEVENT1 = false; //Detecte la presence de modification des label de l'arbre lors d'evenement tel que EU ou EG. Necessaire pour boucler tant qu'on a plus de modifications.

/*
*Constructeur de la classe Kripke.
*Effectue un parsing de file afin de créer le modele Kripke qui y correspond
*
*@Param file 		Fichier txt choisi pour le parsing
*/
Kripke::Kripke(std::string file){
	std::ifstream inputFileStream(file);
	if (inputFileStream){
		std::string line;
		int count = 1;
		while(std::getline(inputFileStream, line)){
			line = line.substr(6);
			line.erase(line.size()-2, 2);
			std::string element;
			std::stringstream lineStream(line);
			std::vector<std::string> tmp;
			while(std::getline(lineStream, element, ',')){
				tmp.push_back(element);
			}
			switch(count){
				case 1: V = tmp;
					break;
				case 2: S = tmp;
					break;
				case 3: S0 = tmp;
					break;
				case 4: {
						std::pair<std::string, std::string> pair;
						std::unordered_map<std::string, std::string> dico;
						for(int unsigned i=0; i<tmp.size(); i++){
							std::string elem = tmp[i];
							pair.second = elem.substr(elem.find(":")+1);
							pair.first = elem.erase(elem.find(":"), elem.size());
							dico.insert(pair);
						}
						I = dico;
					}
					break;
				case 5: {
						std::pair<std::string, std::string> pair;
						std::unordered_map<std::string, std::string> dico;
						for(int unsigned i=0; i<tmp.size(); i++){
							std::string elem = tmp[i];
							pair.second = elem.substr(elem.find(":")+1);
							pair.first = elem.erase(elem.find(":"), elem.size());
							dico.insert(pair);
						}
						R = dico;
					}
					break;
			}
			count += 1;
		}
	}
	initializeNetwork();
}

void Kripke::initializeNetwork(){
	std::string state = getS0(0);
	std::vector<std::string> prop = getI(state);
	root = new Node(state, prop, nullptr);
	buildNetwork(root);
}

bool Kripke::buildNetwork(Node* node){
	std::vector<std::string> states = getR(node->get_state());
	std::vector<std::string>::iterator it;
	bool end = true;
	for(it = states.begin(); it != states.end(); it++){
		if(!isPresent(*it, root)){
			end = false;
		}
	}
	for(it = states.begin(); it != states.end(); it++){
		if (node->get_state() == *it) {
			node->set_father(node);
			node->addSon(node);
		}
		else if (isPresent(*it, root)){
			Node* end = getNode(*it, root);
			end->set_father(node);
			node->addSon(end);
		}
		else {
			new Node(*it, getI(*it), node);
		}
	}
	if (end){
		return true;
	}
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it2;
	for(it2 = sons.begin(); it2 != sons.end(); it2++){
		Node* son = *it2;
		if (son != node){
			buildNetwork(son);
		}
	}
	return true;
}

bool Kripke::isPresent(std::string state, Node* node){
	if(node->get_state() == state){
		return true;
	}
	else {
		std::vector<Node*> sons = node->get_son();
		std::vector<Node*>::iterator it;
		for(it = sons.begin(); it != sons.end(); it++){
			if (*it != node && *it != nullptr){
				return isPresent(state, *it);
			}
		}
	}
	return false;
}

Node* Kripke::getNode(std::string state, Node* node){
	if(node->get_state() == state){
		return node;
	}
	else {
		std::vector<Node*> sons = node->get_son();
		std::vector<Node*>::iterator it;
		for(it = sons.begin(); it != sons.end(); it++){
			if (*it != node && *it != nullptr){
				return getNode(state, *it);
			}
		}
	}
	return nullptr;
}


/*
*Getter de la/les proposition(s) en fonction de l'etat.
*
*@Param indice 			etat du noeud
*@Return 				Renvoie la/les proposition(s) relative(s) a l'etat
*/
std::vector<std::string> Kripke::getI(std::string indice){
	std::string tmp = this->I[indice];
	std::vector<std::string> result;
	std::string element;
	std::stringstream iss(tmp);
	while(std::getline(iss, element, '-')){result.push_back(element);}
	std::vector<std::string>::iterator it;
	for(it = this->V.begin(); it != this->V.end(); it++){
		if(*it != result[0]){
			element = "¬" + *it;
			result.push_back(element);
		}
	result.push_back("true");
	}
	return result;
}

/*
*Getter de la/les relation(s) en fonction de l'etat.
*
*@Param indice 			etat du noeud
*@Return 				Renvoie la/les relation(s) liant un etat a un autre.
*/
std::vector<std::string> Kripke::getR(std::string indice){
	std::string tmp = this->R[indice];
	std::vector<std::string> result;
	std::string element;
	std::stringstream iss(tmp);
	while(std::getline(iss, element, '-')){result.push_back(element);}
	return result;
}


void Kripke::clearChecker(Node* node, std::string checker){
	node->removeLabel(checker);
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != node && son->isFormulaPresent(checker)){
			clearChecker(son, checker);
		}
	}	
}

/*
*Verifie les labels des enfants d'un noeud de depart. 
*Si un enfant contient le label voulu, renvoie true
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*@Return 				True si un enfant contient le label, false sinon
*/
bool Kripke::checkSonsLabel(Node* begin, std::string formula){
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			Node* temp = *it;
			if(temp->isFormulaPresent(formula)){
				return true;
			}
		}
	}
	return false;
}

/*
*Fonction executant la formule CTL "Exist Next". Chez l'un des enfants, ϕ apparait.
*La fonction va donc ajouter le label "EX ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::EX(Node* begin, std::string formula){
	if (checkSonsLabel(begin, formula)){
		begin->addLabel("EX " + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EX "<<formula<<"\""<<std::endl;
	}
	else{
		begin->addLabel("¬EX " + formula);
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != nullptr && son != begin && !son->isFormulaPresent("EX " + formula) && !son->isFormulaPresent("¬EX " + formula)){
			EX(*it, formula);
		}
	}
}

/*
*Premiere etape pour les events "global" (EG). Assigne le label EG(ϕ) aux noeuds comportant le label "ϕ"
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::EGFirstStep(Node* begin, std::string formula){
	if (begin->isFormulaPresent(formula)) {
		begin->addLabel("EG " + formula);	
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EG "<<formula<<"\""<<std::endl;
	}
	else{
		begin->addLabel("¬EG " + formula);	
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != nullptr && son != begin && !son->isFormulaPresent("EG " + formula) && !son->isFormulaPresent("¬EG " + formula)){
			EGFirstStep(*it, formula);
		}
	}
}

/*
*Deuxieme etape pour les events "global" (EG). Retire le label EG(ϕ) aux noeuds n'ayant pas 
*d'enfants avec le label EG(ϕ).
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::EGRemovalStep(Node* begin, std::string formula){
	begin->addLabel("EGChecked");
	if(begin->isFormulaPresent(formula) && !checkSonsLabel(begin, formula)){
		begin->removeLabel(formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" n'a désormais plus le label \""<<formula<<"\""<<std::endl;
		HASCHANGEDDURINGEVENT1 = true;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != nullptr && son != begin && !son->isFormulaPresent("EGChecked")){
			EGRemovalStep(son, formula);
		}
	}
}

/*
*Fonction executant la formule CTL "Exist Global". Dans au moins un chemin, ϕ apparait dans tous les etats futures
*La fonction va donc ajouter le label "EG ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::EG(Node* begin, std::string formula){
	EGFirstStep(begin, formula);
	std::string newFormula = "EG " + formula;
	do{
		HASCHANGEDDURINGEVENT1 = false;
		EGRemovalStep(begin, newFormula);
		clearChecker(begin, "EGChecked");
	} while(HASCHANGEDDURINGEVENT1);
}

/*
*Premiere etape pour les events "union" (EU). Ajoute le label "E[ϕ U ψ]" aux noeuds ayant le label "ψ"
*
*@Param begin 			Noeud de depart
*@Param holdFormula 	Label devant etre present jusqu'a presence du label "endFormula"
*@Param endFormula 		Label present apres le sequence de holdFormula. Peut etre present seul
*/
void Kripke::EUFirstStep(Node* begin, std::string holdFormula, std::string endFormula){
	if (begin->isFormulaPresent(endFormula)) {
		begin->addLabel("E[" + holdFormula + " U " + endFormula + "]");	
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"E["<<holdFormula<<" U "<<endFormula<<"]"<<std::endl;
	}
	else{
		begin->addLabel("¬E[" + holdFormula + " U " + endFormula + "]");	
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != nullptr && son != begin && !son->isFormulaPresent("E[" + holdFormula + " U " + endFormula + "]") && !son->isFormulaPresent("¬E[" + holdFormula + " U " + endFormula + "]")){
			EUFirstStep(son, holdFormula, endFormula);
		}
	}
}

/*
*Deuxieme etape pour les events "union" (EU). Ajoute le label "E[ϕ U ψ]" aux noeuds ayant 
*le label "ϕ" et un enfant avec le label "E[ϕ U ψ]"
*
*@Param begin 			Noeud de depart
*@Param holdFormula 	Label devant etre present jusqu'a presence du label "endFormula"
*@Param endFormula 		Label present apres le sequence de holdFormula. Peut etre present seul
*/
void Kripke::EURemovalStep(Node* begin, std::string holdFormula, std::string endFormula){
	begin->addLabel("EUChecked");
	if(!begin->isFormulaPresent("E[" + holdFormula + " U " + endFormula + "]") && begin->isFormulaPresent(holdFormula) && checkSonsLabel(begin, "E[" + holdFormula + " U " + endFormula + "]")){
		begin->addLabel("E[" + holdFormula + " U " + endFormula + "]");
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"E["<<holdFormula<<" U "<<endFormula<<"]"<<std::endl;
		HASCHANGEDDURINGEVENT1 = true;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != nullptr && son != begin && !son->isFormulaPresent("EUChecked")){
			EURemovalStep(son, holdFormula, endFormula);
		}
	}
}

/*
*Fonction executant la formule CTL "Exist Union". Dans au moins un chemin, ϕ apparait jusqu'a ce que ψ apparaisse.
*La fonction va donc ajouter le label "E[ϕ U ψ]" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param holdFormula 	Label devant etre present jusqu'a presence du label "endFormula"
*@Param endFormula 		Label present apres le sequence de holdFormula. Peut etre present seul
*/
void Kripke::EU(Node* begin, std::string holdFormula, std::string endFormula){
	EUFirstStep(begin, holdFormula, endFormula);
	do{
		HASCHANGEDDURINGEVENT1 = false;
		EURemovalStep(begin, holdFormula, endFormula);
		clearChecker(begin, "EUChecked");
	} while(HASCHANGEDDURINGEVENT1);

}

void Kripke::checkEF(Node* node, std::string formula){
	node->addLabel("EFChecked");
	if(node->isFormulaPresent("E[true U " + formula + "]")){
		node->addLabel("EF " + formula);
		std::cout<<"Le noeud "<<node->get_ID()<<" a désormais le label \"EF "<<formula<<std::endl;
	}
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != node && !son->isFormulaPresent("EFChecked")){
			checkEF(son, formula);
		}
	}	
}

/*
*Fonction executant la formule CTL "Exist Future". Dans au moins un chemin, ϕ apparait dans un état future.
*La fonction va donc ajouter le label "EF ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::EF(Node* begin, std::string formula){
	EU(begin, "true", formula);
	checkEF(begin, formula);
	clearChecker(begin, "EFChecked");
}

void Kripke::notEX(Node* node, std::string formula){
	node->addLabel("AXChecked");
	if(!node->isFormulaPresent("EX ¬" + formula)){
		node->addLabel("AX " + formula);
		std::cout<<"Le noeud "<<node->get_ID()<<" a désormais le label \"AX "<<formula<<std::endl;
	}
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != node && !son->isFormulaPresent("AXChecked")){
			notEX(son, formula);
		}
	}	
}

void Kripke::notEF(Node* node, std::string formula){
	node->addLabel("AGChecked");
	if(!node->isFormulaPresent("EF ¬" + formula)){
		node->addLabel("AG " + formula);
		std::cout<<"Le noeud "<<node->get_ID()<<" a désormais le label \"AG "<<formula<<std::endl;
	}
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != node && !son->isFormulaPresent("AGChecked")){
			notEF(son, formula);
		}
	}	
}

void Kripke::notEG(Node* node, std::string formula){
	node->addLabel("AFChecked");
	if(!node->isFormulaPresent("EG ¬" + formula)){
		node->addLabel("AF " + formula);
		std::cout<<"Le noeud "<<node->get_ID()<<" a désormais le label \"AF "<<formula<<std::endl;
	}
	std::vector<Node*> sons = node->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		Node* son = *it;
		if (son != node && !son->isFormulaPresent("AFChecked")){
			notEG(son, formula);
		}
	}	
}

/*
*Fonction executant la formule CTL "All Next". ϕ apparait dans tous les etats suivants. 
*La fonction va donc ajouter le label "AX ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::AX(Node* begin, std::string formula){
	EX(begin, "¬" + formula);
	notEX(begin, formula);
	clearChecker(begin, "AXChecked");
}

/*
*Fonction executant la formule CTL "All Future". Peu importe le chemin, ϕ apparait dans un état future.
*La fonction va donc ajouter le label "AF ϕ" aux noeuds validant la formule CTL citee plus haut. 
*"All Future" peut en realite s'exprimer comme etant : AF ϕ = ¬EG(¬ϕ).
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::AF(Node* begin, std::string formula){
	EG(begin, "¬" + formula);
	notEG(begin, formula);
	clearChecker(begin, "AFChecked");
}

/*
*Fonction executant la formule CTL "All Global". Peu importe le chemin, ϕ apparait dans tous les états futures.
*La fonction va donc ajouter le label "AG ϕ" aux noeuds validant la formule CTL citee plus haut. 
*"All Global" peut en realite s'exprimer comme etant : AG ϕ = ¬EF(¬ϕ).
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Kripke::AG(Node* begin, std::string formula){
	EF(begin, "¬" + formula);
	notEF(begin, formula);
	clearChecker(begin, "AGChecked");
}

/*
*Fonction executant la formule CTL "All Union".  Dans tous les chemins, ϕ apparait jusqu'à ce que ψ apparaisse.
*La fonction va donc ajouter le label "AU[ϕ U ψ]" aux noeuds validant la formule CTL citee plus haut. 
*"All Union" peut en realite s'exprimer comme etant : AU[ϕ U ψ] = ¬E[¬ψ U (¬ϕ ∧ ¬ψ)] ∧ ¬EG(¬ψ)
*
*ATTENTION : NE FONCTIONNE PAS
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
//void Kripke::AU(Node* begin, std::string holdFormula, std::string endFormula){
	//EG(begin, neg(endFormula));

//}