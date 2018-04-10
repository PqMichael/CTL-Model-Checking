#include <iostream>
#include <string>
#include "node.hpp"
#include "tree.hpp"
#include "kripke.hpp"
#include <vector>

int main(){
	Kripke* K = new Kripke("data.txt");
	Tree* A = new Tree(K); //ID racine = 0
	Node* root = A->get_root();
	Node* myNode = A->findNode(root, 10);
	std::vector<std::string> myProp {"fermé", "¬se ferme", "¬ouvert", "true"};
	myNode->set_prop(myProp);

	std::cout<<"\n\n----EXECUTION EF(fermé)----\n\n";
	A->EF(root, "fermé");

	std::cout<<"\n\n----EXECUTION EX(fermé)----\n\n";
	A->EX(root, "fermé");

	std::cout<<"\n\n----EXECUTION EG(fermé)----\n\n";
	A->EG(root, "fermé");

	std::cout<<"\n\n----EXECUTION E[ouvert U se ferme]----\n\n";
	A->EU(root, "ouvert", "se ferme");

	std::cout<<"\n\n----EXECUTION AX(fermé)----\n\n";
	A->AX(root, "fermé");

	std::cout<<"\n\n----EXECUTION AF(fermé)----\n\n";
	A->AF(root, "fermé");

	std::cout<<"\n\n----EXECUTION AG(fermé)----\n\n";
	A->AG(root, "fermé");

	//std::cout<<"\n\n----EXECUTION A[ouvert U se ferme]----\n\n";
	//A->AU(root, "ouvert", "se ferme");

	return 0;
}