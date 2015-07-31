#include <iostream>
#include "Graph.h"

using namespace graph;

Node::Node(){}

Node::Node(int _Id, std::string _Type, std::string _IP, std::string _Location)
{
	Id = _Id;
	Type = _Type;
	IP = _IP;
	Location = _Location;
}

Node::~Node(){}