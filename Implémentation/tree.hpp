#ifndef _TREE_HPP_
#define _TREE_HPP_
#include "node.hpp"
#include "kripke.hpp"
#include <string>

class Tree{
private:

	Node* m_root;

public:

	Tree(Kripke*);
	Tree(const Tree&) = default;
	~Tree() = default;

private:

	
	void buildTree(Kripke*, int, Node*);
	Node* addNode(int, std::string, std::vector<std::string>);
	bool checkSuccessorsLabel(Node*, std::string);
	bool checkSonsLabel(Node*, std::string);
	bool checkAllSonsLabel(Node*, std::string);
	void existGlobalFirstStep(Node*, std::string);
	void allGlobalFirstStep(Node*, std::string);
	void globalRemovalStep(Node*, std::string);
	void unionFirstStep(Node*, std::string, std::string);
	void unionSecondStep(Node*, std::string, std::string);
	void notEG(Node*, std::string);
	void notEF(Node*, std::string);

public:

	inline Node* get_root()	const{return this->m_root;}
	Node* findNode(Node*, int);

	void EX(Node*, std::string);
	void EF(Node*, std::string);
	void EG(Node*, std::string);
	void EU(Node*, std::string, std::string);

	void AX(Node*, std::string);
	void AF(Node*, std::string);
	void AG(Node*, std::string);
	void AU(Node*, std::string, std::string);

	std::string neg(std::string);

};

#endif