#ifndef _KRIPKE_HPP_
#define _KRIPKE_HPP_
#include <string>
#include <vector>
#include "node.hpp"
#include <unordered_map>

class Kripke{
private:
	std::vector<std::string> V;
	std::vector<std::string> S;
	std::vector<std::string> S0;
	std::unordered_map<std::string, std::string> I;
	std::unordered_map<std::string, std::string> R;
	Node* root;
	
public:
	Kripke(std::string);
	Kripke(const Kripke&) = default;
	~Kripke() = default;
	inline std::string getV(int indice)        const{return this->V[indice];}
	inline std::string getS(int indice)        const{return this->S[indice];}
	inline std::string getS0(int indice)       const{return this->S0[indice];}
	inline Node* getRoot()					   const{return this->root;}
	std::vector<std::string> getI(std::string);
	std::vector<std::string> getR(std::string);

	void initializeNetwork();
	bool buildNetwork(Node*);
	bool isPresent(std::string, Node*);
	Node* getNode(std::string, Node*);
	void clearChecker(Node*, std::string);

	bool checkSonsLabel(Node*, std::string);

	void EGFirstStep(Node*, std::string);
	void EGRemovalStep(Node*, std::string);

	void EUFirstStep(Node*, std::string, std::string);
	void EURemovalStep(Node*, std::string, std::string);

	void checkEF(Node*, std::string);
	void notEX(Node*, std::string);
	void notEG(Node*, std::string);
	void notEF(Node*, std::string);

	void EX(Node*, std::string);
	void EG(Node*, std::string);
	void EU(Node*, std::string, std::string);
	void EF(Node*, std::string);

	void AX(Node*, std::string);
	void AG(Node*, std::string);
	void AU(Node*, std::string, std::string);
	void AF(Node*, std::string);
};

#endif