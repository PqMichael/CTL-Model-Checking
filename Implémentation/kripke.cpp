#include "kripke.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>


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