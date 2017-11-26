#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <vector>

static const int NB_PROP = 2; //Nombre de proprietes dont fait part le modèle

enum State{un, deux, trois};

class Node
{
private:
	bool tab[NB_PROP];
	Node* m_father;
	std::vector<Node*> m_son;
	std::vector<Node*>::iterator it;
	State s;
	int ID;
public:
	Node(State, bool, bool, Node*);
	Node(const Node&) = default;
	~Node() = default; //To be modified
	inline bool getTab(int indice)     const{return this->tab[indice];}
	inline Node* get_father()          const{return this->m_father;}
	inline std::vector<Node*> get_son()const{return this->m_son;}
	inline State get_state()           const{return this->s;}
	inline int get_ID()                const{return this->ID;}
	void set_state(int, bool);
	void addSon(Node*);
};


#endif