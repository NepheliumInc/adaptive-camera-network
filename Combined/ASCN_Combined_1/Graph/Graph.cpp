#include <iostream>
#include "Graph.h"
#include <stdexcept>
#include <algorithm>

using namespace graph;

Graph::Graph(){}

bool Graph::isConfigured()
{
	return true;
}

int Graph::initGraph()
{
	addNode(Node("C001", "start", "1.1.1.1", "GR1"));
	addNode(Node("C002", "normal", "1.1.1.2", "GR2"));
	addNode(Node("C003", "normal", "1.1.1.3", "GR3"));

	addNeighbour("C001", "C002", 8, 1);
	addNeighbour("C002", "C001", 2, 1);

	addNeighbour("C001", "C003", 6, 1);
	addNeighbour("C003", "C001", 4, 1);

	cout << "Graph initiated. \n";
	return Nodes.size();
}

vector<Node> Graph::getAllNodes()
{
	vector<Node> returnVector;
	for (map<string, Node>::const_iterator i = Nodes.begin(); i != Nodes.end(); i++)
	{
		returnVector.push_back(i->second);
	}
	return returnVector;
}

vector<string> Graph::getKeysOfAllNodes()
{
	vector<string> returnVector;
	for (map<string, Node>::const_iterator i = Nodes.begin(); i != Nodes.end(); i++)
	{
		returnVector.push_back(i->first);
	}
	return returnVector;
}

Node * Graph::getNode(string node_id)
{
	return &Nodes.at(node_id);
}

bool Graph::addNode(Node node)
{
	Nodes.insert(map<string, Node>::value_type(node.Id, node));
	vector<vector<pair<string, int>>> vect(9);
	Edges[node.Id] = vect;
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

bool Graph::deleteNode(string node_id)
{
	Nodes.erase(node_id);
	return false;
}

map<string, vector<vector<pair<string, int>>>> Graph::getAdgecencyList()
{
	return Edges;
}

vector<vector<pair<string, int>>> Graph::getNeighbours(string base_node_id)
{
	return Edges.at(base_node_id);
}

bool Graph::addNeighbour(string base_node_id, Node new_node, int location_in_metrix, int weight)
{
	if (Nodes.count(new_node.Id) == 0)
	{
		Graph::addNode(new_node);
	}
	else
	{
		cout << "# ERROR :: at addNeighbour - new node is already exist. Use other override of addNeighbour.\n";
		return false;
	}
	return addNeighbour(base_node_id, new_node.Id, location_in_metrix, weight);
}

bool Graph::addNeighbour(string base_node_id, string new_node_id, int location_in_metrix, int weight)
{
	if (Nodes.count(base_node_id) != 0 && Nodes.count(new_node_id) != 0)	// if both nodes are available
	{
		if (Edges.count(base_node_id) != 0)	// if base node is already in the edges collection
		{
			for (vector<pair<string, int>>::iterator i = Edges[base_node_id][location_in_metrix].begin(); i != Edges[base_node_id][location_in_metrix].end(); i++)
			{
				if ((*i).first == new_node_id)
				{
					(*i).second = weight;
					return true;
				}
			}
			Edges[base_node_id][location_in_metrix].push_back(vector<pair<string, int>>::value_type(make_pair(new_node_id, weight)));
			return true;
		}
		else
		{
			Edges[base_node_id][location_in_metrix].push_back(vector<pair<string, int>>::value_type(make_pair(new_node_id, weight)));
			return true;
		}
	}
	else
	{
		cout << "# ERROR :: at addNeighbour - node ids are not exist. Add nodes before continue.\n";
		return false;
	}
}

bool Graph::removeNeighbour(string base_node_id, string removing_id)
{
	for (vector<vector<pair<string, int>>>::iterator i = Edges.at(base_node_id).begin(); i != Edges.at(base_node_id).end(); i++)
		for (vector<pair<string, int>>::iterator j = (*i).begin(); j != (*i).end(); j++)
			if ((*j).first == removing_id)
				(*i).erase(j);
	return true;
}

bool Graph::isNeighbour(string node1_id, string node2_id)
{
	for (vector<vector<pair<string, int>>>::iterator i = Edges.at(node1_id).begin(); i != Edges.at(node1_id).end(); i++)
		for (vector<pair<string, int>>::iterator j = (*i).begin(); j != (*i).end(); j++)
			if ((*j).first == node2_id)
				return true;
	return false;
}


Graph::~Graph(){}


