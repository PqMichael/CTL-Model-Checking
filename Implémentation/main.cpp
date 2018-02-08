#include <iostream>
#include "node.hpp"
#include "tree.hpp"
#include "kripke.hpp"

int N_NODE = 0;

int main(){
	std::cout<<"oui"<<std::endl;
	Kripke* K = new Kripke("data.txt");
	std::cout<<"oui"<<std::endl;
	Tree* A = new Tree(K); //ID racine = 0
	return 0;
}