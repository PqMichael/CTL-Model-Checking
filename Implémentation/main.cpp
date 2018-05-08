#include <iostream>
#include <string>
#include "node.hpp"
#include "tree.hpp"
#include "kripke.hpp"
#include <vector>

int main(){
	Kripke* K = new Kripke("data.txt");


	std::cout<<"\n\n----EXECUTION EX(fermé)----\n\n";
	K->EX(K->getRoot(), "fermé");

	std::cout<<"\n\n----EXECUTION EG(fermé)----\n\n";
	K->EG(K->getRoot(), "fermé");

	std::cout<<"\n\n----EXECUTION E[ouvert U se ferme]----\n\n";
	K->EU(K->getRoot(), "ouvert", "se ferme");

	std::cout<<"\n\n----EXECUTION EF(fermé)----\n\n";
	K->EF(K->getRoot(), "fermé");

	std::cout<<"\n\n----EXECUTION AX(fermé)----\n\n";
	K->AX(K->getRoot(), "fermé");

	std::cout<<"\n\n----EXECUTION AF(fermé)----\n\n";
	K->AF(K->getRoot(), "fermé");

	std::cout<<"\n\n----EXECUTION AG(fermé)----\n\n";
	K->AG(K->getRoot(), "fermé");

	//Tree* A = new Tree(K); Création d'un arbre

	return 0;
}