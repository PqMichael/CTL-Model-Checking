#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <vector>
#include <string>

class Node
{
private:
	std::vector<std::string> prop;
	Node* m_father;
	std::vector<Node*> m_son;
	std::vector<Node*>::iterator it;
	std::string s;
	int ID;
public:
	Node(std::string, std::vector<std::string>, Node*);
	Node(const Node&) = default;
	~Node() = default; //To be modified
	inline std::vector<std::string> getTab()     const{return this->prop;}
	inline Node* get_father()          const{return this->m_father;}
	inline std::vector<Node*> get_son()const{return this->m_son;}
	inline std::string get_state()     const{return this->s;}
	inline int get_ID()                const{return this->ID;}

	void addSon(Node*);
};


#endif