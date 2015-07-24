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
	return true;
}

bool Graph::editNode(Node node)
{
	Nodes[node.Id].IP = node.IP;
	Nodes[node.Id].Location = node.Location;
	Nodes[node.Id].PossibleProfiles = node.PossibleProfiles;
	Nodes[node.Id].Type = node.Type;
	return true;
}

bool Graph::deleteNode(int node_id)
{
	Nodes.erase(node_id);
	return false;
}

std::map<int, std::map<int, EdgedNode>> Graph::getAdgecencyList()
{
	return Graph::Edges;
}

std::map<int, EdgedNode> Graph::getNeighbours(int base_node_id)
{
	return Graph::Edges[base_node_id];
}

bool Graph::addNeighbour(int base_node_id, Node new_node, int location_in_metrix)
{
	Graph::addNode(new_node);

	return true;
}

bool Graph::addNeighbour(int base_node_id, int new_node_id, int location_in_metrix)
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


