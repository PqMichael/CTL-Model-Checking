#ifndef _KRIPKE_HPP_
#define _KRIPKE_HPP_
#include <string>
#include <vector>
#include <unordered_map>

class Kripke{
private:
	std::vector<std::string> V;
	std::vector<std::string> S;
	std::vector<std::string> S0;
	std::unordered_map<std::string, std::string> I;
	std::unordered_map<std::string, std::string> R;
public:
	Kripke(std::string);
	Kripke(const Kripke&) = default;
	~Kripke() = default;
	inline std::string getV(int indice)        const{return this->V[indice];}
	inline std::string getS(int indice)        const{return this->S[indice];}
	inline std::string getS0(int indice)       const{return this->S0[indice];}
	std::vector<std::string> getI(std::string);
	std::vector<std::string> getR(std::string);

};

#endif