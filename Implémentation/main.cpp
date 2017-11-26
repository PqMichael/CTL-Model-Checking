#include <iostream>
#include "node.hpp"
#include "tree.hpp"

int N_NODE = 0;

int main(){
	Tree* A = new Tree(); //ID racine = 0
	A->addNode(0, deux, false, true); //ID = 1
	A->addNode(0, un, true, false);   //ID = 2
	A->addNode(1, trois, true, true); //ID = 3
	A->addNode(3, un, true, false);   //ID = 4
	A->addNode(4, deux, false, true); //ID = 5
	A->addNode(2, deux, false, true); //ID = 6
	A->addNode(8, trois, true, true); //Génère une erreur car le noeud n°8 n'existe pas
	return 0;
}