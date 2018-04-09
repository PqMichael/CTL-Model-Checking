#include <iostream>
#include "node.hpp"
#include "tree.hpp"
#include "kripke.hpp"

int main(){
	Kripke* K = new Kripke("data.txt");
	Tree* A = new Tree(K); //ID racine = 0
	Node* root = A->get_root();

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


	return 0;
}