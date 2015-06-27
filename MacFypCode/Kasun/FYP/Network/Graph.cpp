#include <iostream>
#include "Model.h"

using namespace model;

Graph::Graph(){}

std::vector<Node> Graph::getAllNodes()
{
	std::vector<Node> returnVector;
	for (std::map<int, Node>::const_iterator i = Nodes.begin(); i != Nodes.end(); i++)
	{
		returnVector.push_back(i->second);
	}
	return returnVector;
}

Node Graph::getNode(int node_id)
{
	Node returnNode;
	returnNode = Nodes[node_id];
	return returnNode;
}

bool Graph::addNode(Node node)
{
	Nodes.insert(std::map<int, Node>::value_type(node.Id, node));
	return false;
}

bool Graph::editNode(Node node)
{
	return false;
}

bool Graph::deleteNode(int node_id)
{
	return false;
}

std::map<int, std::vector<int>> Graph::getAdgecencyList()
{
	std::map<int, std::vector<int>> returnMap;
	return returnMap;
}

std::vector<int> Graph::getNeighbours(int base_node_id)
{
	std::vector<int> returnVector;
	return returnVector;
}

bool Graph::addNeighbour(int base_node_id, Node new_node)
{
	return false;
}

bool Graph::addNeighbour(int base_node_id, int new_node_id)
{
	return false;
}

bool Graph::removeNeighbour(int base_node_id, int removing_id)
{
	return false;
}

bool Graph::isNeighbour(int node1_id, int node2_id)
{
	return false;
}


Graph::~Graph(){}


