#include <iostream>
#include "Graph.h"
#include <stdexcept>

using namespace graph;

Graph::Graph(){}

bool Graph::isConfigured()
{
	return true;
}

void Graph::initGraph()
{

}

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
	returnNode = Nodes.at(node_id);
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
	return Graph::Edges.at(base_node_id);
}

bool Graph::addNeighbour(int base_node_id, Node new_node, int location_in_metrix, int weight)
{
	if (Graph::Nodes.count(new_node.Id) == 0)
	{
		Graph::addNode(new_node);
	}
	else
	{
		std::cout << "# ERROR :: at addNeighbour - new node is already exist. Use other override of addNeighbour.\n";
		return false;
	}
	if (Graph::Nodes.count(base_node_id) != 0)
	{
		Graph::Edges.at(base_node_id).at(location_in_metrix).WeightedNodes.push_back(std::make_pair(new_node.Id, weight));
		return true;
	}
	else
	{
		std::cout << "# ERROR :: at addNeighbour - base node is not exist. Add basenode before continue.\n";
		return false;
	}
}

bool Graph::addNeighbour(int base_node_id, int new_node_id, int location_in_metrix, int weight)
{
	if (Graph::Nodes.count(base_node_id) != 0)
	{
		Graph::Edges.at(base_node_id).at(location_in_metrix).WeightedNodes.push_back(std::make_pair(new_node_id, weight));
		return true;
	}
	else
	{
		std::cout << "# ERROR :: at addNeighbour - base node is not exist. Add basenode before continue.\n";
		return false;
	}
}

bool Graph::removeNeighbour(int base_node_id, int removing_id)
{
	for (std::map<int, EdgedNode>::iterator i = Graph::Edges.at(base_node_id).begin(); i != Graph::Edges.at(base_node_id).end(); i++)
		for (std::vector<std::pair<int, int>>::iterator j = (*i).second.WeightedNodes.begin(); j != (*i).second.WeightedNodes.end(); j++)
			if ((*j).first == removing_id)
				(*i).second.WeightedNodes.erase(j);
	return true;
}

bool Graph::isNeighbour(int node1_id, int node2_id)
{
	for (std::map<int, EdgedNode>::iterator i = Graph::Edges.at(node1_id).begin(); i != Graph::Edges.at(node1_id).end(); i++)
		for (std::vector<std::pair<int, int>>::iterator j = (*i).second.WeightedNodes.begin(); j != (*i).second.WeightedNodes.end(); j++)
			if ((*j).first == node2_id)
				return true;
	return false;
}


Graph::~Graph(){}


