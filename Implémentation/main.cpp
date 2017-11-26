#include <iostream>
#include "node.hpp"

int main(){
	Node x = Node(5, 6, nullptr);
	Node y = Node(1, 2, &x);
	std::cout<<x.get_son()[0]<<std::endl;
	std::cout<<y.get_father()<<std::endl;
	return 0;
}