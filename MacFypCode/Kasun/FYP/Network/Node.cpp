#include <iostream>
#include "Model.h"

using namespace model;

Node::Node(){}

Node::Node(int _Id, std::string _Type, std::string _IP, std::string _Location)
{
	Id = _Id;
	Type = _Type;
	IP = _IP;
	Location = _Location;
}

Node::~Node(){}