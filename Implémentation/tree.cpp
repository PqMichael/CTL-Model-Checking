#include "tree.hpp"
#include "node.hpp"
#include "kripke.hpp"
#include <iostream>
#include <string>

int SIZEOFTREE = 3; //Taille de l'arbre
bool HASCHANGEDDURINGEVENT = false; //Detecte la presence de modification des label de l'arbre lors d'evenement tel que EU ou EG. Necessaire pour boucler tant qu'on a plus de modifications.


/*
*Constructeur de la classe Tree sur base d'un modele Kripke
*
*@Param K 				Modele Kripke
*/
Tree::Tree(Kripke* K){
	std::string state = K->getS0(0);
	std::vector<std::string> prop = K->getI(state);
	m_root = new Node(state, prop, nullptr);
	buildTree(K, 0, m_root);
}

/*
*Construit de façon récusrive l'arbre sur base du modèle Kripke et de la taille voulue.
*
*@Param K 				Modele Kripke
*@Param step 			Taille de l'arbre voulue
*@Param node 			Noeud de depart, necessaire pour la recursion
*/
void Tree::buildTree(Kripke* K, int step, Node* node){
	std::vector<std::string> states = K->getR(node->get_state());
	std::vector<std::string>::iterator it;
	for(it = states.begin(); it != states.end(); it++){
		Node* result = addNode(node->get_ID(), *it, K->getI(*it));
		if (step < SIZEOFTREE) {
			buildTree(K, step+1, result);
		}
	}
}

/*
*Ajoute un noeud en dessous d'un autre (son pere). La fonction
*va donc chercher le pere pour ensuite y ajouter un enfant.
*
*@Param father_ID 		ID du noeud pere
*@Param s 				etat du noeud que l'on cree
*@Param prop 			propositions du noeud que l'on cree
*@Return 				Renvoie le noeud que l'on a cree
*/
Node *Tree::addNode(int father_ID, std::string s, std::vector<std::string> prop){
	Node* father = findNode(this->m_root, father_ID);
	if (father == nullptr) {
		std::cout<<"Le demande était de créer le noeud attaché à l'ID "<<father_ID<<std::endl;
		std::cout<<"ID non trouvé, noeud supprimé"<<std::endl;
		return nullptr;
	}
	else {
		return new Node(s, prop, father);
	}
}

/*
*Cherche le noeud portant l'ID voulu en partant d'un noeud voulu. Il s'agit
*d'une recherche recusrive dans l'arbre.
*
*@Param begin  			Noeud de depart
*@Param ID 				ID du noeud qu'on recherche
*@Return 				Renvoie le noeud qu'on recherche, nullptr s'il n'existe pas
*/
Node* Tree::findNode(Node* begin, int ID){
	Node* temp = begin;
	if (temp->get_ID() == ID) {
		return temp;
	}
	else{
		std::vector<Node*> sons = temp->get_son();
		std::vector<Node*>::iterator it;
		for(it = sons.begin(); it != sons.end(); it++){
			if (*it != nullptr){
				Node* result = findNode(*it, ID);
				if (result != nullptr) {
					return result;
				}
			}
		}
	}
	return nullptr;
}

/*
*Verifie les labels de tous les successeurs d'un noeud de depart. 
*Si un successeur contient le label voulu, renvoie true
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*@Return 				True si un successeur contient le label, false sinon
*/
bool Tree::checkSuccessorsLabel(Node* begin, std::string formula){
	if (checkSonsLabel(begin, formula)) {
		return true;
	}
	else{
		std::vector<Node*> sons = begin->get_son();
		std::vector<Node*>::iterator it;
		for(it = sons.begin(); it != sons.end(); it++){
			if (*it != nullptr){
				if (checkSuccessorsLabel(*it, formula)) {
					return true;
				}
			}
		}
	}
	return false;
}

/*
*Verifie les labels des enfants d'un noeud de depart. 
*Si un enfant contient le label voulu, renvoie true
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*@Return 				True si un enfant contient le label, false sinon
*/
bool Tree::checkSonsLabel(Node* begin, std::string formula){
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
*Verifie les labels des enfants d'un noeud de depart. 
*Si tous les enfants contiennent le label voulu, renvoie true
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*@Return 				True si tous les enfants contiennent le label, false sinon
*/
bool Tree::checkAllSonsLabel(Node* begin, std::string formula){
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			Node* temp = *it;
			if(!temp->isFormulaPresent(formula)){
				return false;
			}
		}
		else{
			return false;
		}
	}
	return true;
}


/*
*Premiere etape pour les events "global" (EG). Assigne le label EG(ϕ) aux noeuds comportant le label "ϕ"
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::existGlobalFirstStep(Node* begin, std::string formula){
	if (begin->isFormulaPresent(formula)) {
		begin->addLabel("EG " + formula);	
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EG "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			existGlobalFirstStep(*it, formula);
		}
	}
}

/*
*Premiere etape pour l'event "all global" (AG). Assigne le label AG(ϕ) aux noeuds comportant le label "ϕ"
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::allGlobalFirstStep(Node* begin, std::string formula){
	if (!begin->isFormulaPresent(formula)) {
		begin->addLabel("EG ¬" + formula);	
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EG ¬"<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			allGlobalFirstStep(*it, formula);
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
void Tree::globalRemovalStep(Node* begin, std::string formula){
	if(begin->isFormulaPresent(formula) && begin->get_son()[0] == nullptr){
		begin->removeLabel(formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" n'a désormais plus le label \""<<formula<<"\""<<std::endl;
		HASCHANGEDDURINGEVENT = true;
	}
	else if(begin->isFormulaPresent(formula) && !checkSonsLabel(begin, formula) && begin->get_son()[0]->get_son()[0] != nullptr){
		begin->removeLabel(formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" n'a désormais plus le label \""<<formula<<"\""<<std::endl;
		HASCHANGEDDURINGEVENT = true;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			globalRemovalStep(*it, formula);
		}
	}
}

/*
*Premiere etape pour les events "union" (EU). Ajoute le label "E[ϕ U ψ]" aux noeuds ayant le label "ψ"
*
*@Param begin 			Noeud de depart
*@Param holdFormula 	Label devant etre present jusqu'a presence du label "endFormula"
*@Param endFormula 		Label present apres le sequence de holdFormula. Peut etre present seul
*/
void Tree::unionFirstStep(Node* begin, std::string holdFormula, std::string endFormula){
	if (begin->isFormulaPresent(endFormula)) {
		begin->addLabel("E[" + holdFormula + " U " + endFormula + "]");	
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"E["<<holdFormula<<" U "<<endFormula<<"]"<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			unionFirstStep(*it, holdFormula, endFormula);
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
void Tree::unionSecondStep(Node* begin, std::string holdFormula, std::string endFormula){
	if(!begin->isFormulaPresent("E[" + holdFormula + " U " + endFormula + "]") && begin->isFormulaPresent(holdFormula) && checkSonsLabel(begin, "E[" + holdFormula + " U " + endFormula + "]")){
		begin->addLabel("E[" + holdFormula + " U " + endFormula + "]");
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"E["<<holdFormula<<" U "<<endFormula<<"]"<<std::endl;
		HASCHANGEDDURINGEVENT = true;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			unionSecondStep(*it, holdFormula, endFormula);
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
void Tree::EF(Node* begin, std::string formula){
	if (checkSuccessorsLabel(begin, formula)){
		begin->addLabel("EF " + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EF "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			EF(*it, formula);
		}
	}
}

/*
*Fonction executant la formule CTL "Exist Next". Chez l'un des enfants, ϕ apparait.
*La fonction va donc ajouter le label "EX ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::EX(Node* begin, std::string formula){
	if (checkSonsLabel(begin, formula)){
		begin->addLabel("EX" + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"EX "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			EX(*it, formula);
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
void Tree::EG(Node* begin, std::string formula){
	existGlobalFirstStep(begin, formula);
	std::string newFormula = "EG " + formula;
	do{
		HASCHANGEDDURINGEVENT = false;
		globalRemovalStep(begin, newFormula);
	} while(HASCHANGEDDURINGEVENT);
}

/*
*Fonction executant la formule CTL "Exist Union". Dans au moins un chemin, ϕ apparait jusqu'a ce que ψ apparaisse.
*La fonction va donc ajouter le label "E[ϕ U ψ]" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param holdFormula 	Label devant etre present jusqu'a presence du label "endFormula"
*@Param endFormula 		Label present apres le sequence de holdFormula. Peut etre present seul
*/
void Tree::EU(Node* begin, std::string holdFormula, std::string endFormula){
	unionFirstStep(begin, holdFormula, endFormula);
	do{
		HASCHANGEDDURINGEVENT = false;
		unionSecondStep(begin, holdFormula, endFormula);
	} while(HASCHANGEDDURINGEVENT);

}

/*
*Fonction executant la formule CTL "All Next". ϕ apparait dans tous les etats suivants. 
*La fonction va donc ajouter le label "AX ϕ" aux noeuds validant la formule CTL citee plus haut. 
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::AX(Node* begin, std::string formula){
	if (checkAllSonsLabel(begin, formula)){
		begin->addLabel("AX" + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"AX "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			AX(*it, formula);
		}
	}
}

/*
*Fonction executant la formule CTL "All Future". Peu importe le chemin, ϕ apparait dans un état future.
*La fonction va donc ajouter le label "AF ϕ" aux noeuds validant la formule CTL citee plus haut. 
*"All Future" peut en realite s'exprimer comme etant : AF ϕ = ¬EG(¬ϕ).
*
*ATTENTION : NE FONCTIONNE PAS
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::AF(Node* begin, std::string formula){
	EG(begin, neg(formula));
	notEG(begin, formula);
}

/*
*Si un noeud n'a pas EG(¬ϕ), il a donc ¬EG(¬ϕ) et donc AF ϕ. La fonction va parcourir l'arbre 
*pour ajouter ces labels le cas echeant.
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::notEG(Node* begin, std::string formula){
	if(!begin->isFormulaPresent("EG " + neg(formula)) && begin->get_son()[0] != nullptr){
		begin->addLabel("AF " + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"AF "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			notEG(*it, formula);
		}
	}
}

/*
*Fonction executant la formule CTL "All Global". Peu importe le chemin, ϕ apparait dans tous les états futures.
*La fonction va donc ajouter le label "AG ϕ" aux noeuds validant la formule CTL citee plus haut. 
*"All Global" peut en realite s'exprimer comme etant : AG ϕ = ¬EF(¬ϕ).
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::AG(Node* begin, std::string formula){
	EF(begin, neg(formula));
	notEF(begin, formula);
}

/*
*Si un noeud n'a pas EF(¬ϕ), il a donc ¬EF(¬ϕ) et donc AG ϕ. La fonction va parcourir l'arbre 
*pour ajouter ces labels le cas echeant.
*
*@Param begin 			Noeud de depart
*@Param formula 		Label voulu
*/
void Tree::notEF(Node* begin, std::string formula){
	if(!begin->isFormulaPresent("EF ¬" + formula) && begin->get_son()[0] != nullptr && begin->isFormulaPresent(formula)){
		begin->addLabel("AG " + formula);
		std::cout<<"Le noeud "<<begin->get_ID()<<" a désormais le label \"AG "<<formula<<"\""<<std::endl;
	}
	std::vector<Node*> sons = begin->get_son();
	std::vector<Node*>::iterator it;
	for(it = sons.begin(); it != sons.end(); it++){
		if (*it != nullptr){
			notEF(*it, formula);
		}
	}
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
void Tree::AU(Node* begin, std::string holdFormula, std::string endFormula){
	//EG(begin, neg(endFormula));

}

/*
*Fonction qui negate la formule
*
*@Param formula 		Label voulu
*/
std::string Tree::neg(std::string formula){
	std::string result = "¬" + formula;
	return result;
}
