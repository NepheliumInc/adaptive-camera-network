#pragma once
#include <vector>
#include <map>

namespace model{
	class Graph;
	class Node;
	class EdgedNode;

	class Graph
	{
	protected:
		//collection of nodes as <node_id, node_obj>
		std::map<int, Node> Nodes;

		//collection of edges as <base_node_id, map<location_in_viewport, adgecent_nodes>>
		std::map<int, std::map<int, EdgedNode>> Edges;

	public:
		Graph();
		~Graph();
		/*
		get all nodes in Graph space.
		returns	: vector<model::Node objects>
		*/
		std::vector<Node> getAllNodes();

		/*
		get Node from id.
		returns	: model::Node object - exception if not found
		params	: node id (integer)
		*/
		Node getNode(int node_id);
		
		/*
		add Node to Graph::Nodes collection.
		returns	: boolian value (true if success, false if error in process) - exception if Node.Id already exist
		params	: model::Node object
		*/
		bool addNode(Node node);
		
		/*
		edit a Node in Graph::Nodes collection.
		returns	: boolian value (true if success, false if error in process) - exception if Node not found
		params	: model::Node object
		*/
		bool editNode(Node node);
		
		/*
		delete a Node from Graph::Nodes collection.
		returns	: boolian value (true if success, false if error in process) - exception if relevent Node not found
		params	: node id (integer)
		*/
		bool deleteNode(int node_id);
		
		/*
		get the adgecency list for the graph.
		returns	: map<base node id, vector<adgecent node id>> - NULL if not any
		*/
		std::map<int, std::map<int, EdgedNode>> getAdgecencyList();
		
		/*
		get all adgecent nodes' ids of a given node.
		returns	: vector<adgecent node id> - NULL if not any - exception if base node id not found
		params	: base node id (integer)
		*/
		std::map<int, EdgedNode> getNeighbours(int base_node_id);
		
		/*
		add new model::Node as an adgecent node to the given node.
		returns	: boolian (true if success, false otherwise) - exception if base node not found or new node id already exist in node space
		params	: base node id (integer), new node object(model::Node)
		*/
		bool addNeighbour(int base_node_id, Node new_node, int location_in_metrix);
		
		/*
		add existing model::Node as an adgecent node to the given node.
		returns	: boolian (true if success, false otherwise) - exception if base node or new node not found
		params	: base node id (integer), new node id(integer)
		*/
		bool addNeighbour(int base_node_id, int new_node_id, int location_in_metrix);
		
		/*
		remove a model::Node from the adgecent node list of the given node.
		returns	: boolian (true if success, false otherwise) - exception if base node or removing node not found
		params	: base node id (integer), removing node id(integer)
		*/
		bool removeNeighbour(int base_node_id, int removing_id);
		
		/*
		check if two model::Node s are adgecent.
		returns	: boolian (true if success, false otherwise) - exception if nodes not found
		params	: node1 id (integer), node2 id(integer)
		*/
		bool isNeighbour(int node1_id, int node2_id);
	};


	class Node
	{
	public:
		Node();
		Node(int _Id, std::string _Type, std::string _IP, std::string _Location);
		~Node();

		int Id;
		std::string Type; // start, end, door, normal
		std::string IP;
		std::string Location; //physical
		std::vector<int> PossibleProfiles;
	};


	class EdgedNode
	{
	public:
		EdgedNode();
		EdgedNode(int _id, int _loc);
		~EdgedNode();

		int NodeId; // id of the adjecent node
		std::vector<std::pair<int, int>> WeightedNodes;
	};

}