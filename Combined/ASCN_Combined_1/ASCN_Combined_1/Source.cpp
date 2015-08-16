/*
	Authors: K D K Madusanka [kdk.madusanka@gmail.com]

	Description:
		This is the main source code for the "Adaptive Survellance Camera Networks" project.
		All other suppotive functionalities are referenced.
		The main program structure and multi-threading happening in here.

	Log:
*/


#include <stdio.h>
#include <iostream>

#include <Windows.h> // threading purpose
#include <process.h>

#include "Graph.h"



unsigned int __stdcall mythread(void* data);

graph::Graph _graph;

int main(int argc, char* argv[])
{
	std::cout << "Advanced Surveillance Camera Network (ASCN)\nInitialting ...\n";

	std::cout << "Network Graph checking ...\n";

	_graph = graph::Graph();

	int noNodes = _graph.initGraph();

	if (!(_graph.isConfigured()))
	{
		// cofigure graph.
		_graph.addNode(graph::Node("C001", "start", "1.1.1.1", "GR1"));
		_graph.addNode(graph::Node("C002", "normal", "1.1.1.2", "GR2"));
		_graph.addNeighbour("C001", "C002", 8, 10);
		_graph.addNeighbour("C002", "C001", 2, 10);
		_graph.addNeighbour("C001", "C003", 6, 1);
		_graph.addNeighbour("C003", "C001", 4, 1);
	}


	// config graph with process ids - initialte threds and assign thred id to nodes in graph
	vector<string> nodeKeys = _graph.getKeysOfAllNodes();
	
	for (int i = 0; i < nodeKeys.size(); i++)
	{
		const char *s = nodeKeys[i].c_str();
		_beginthreadex(NULL, 0, &mythread, (void*)&(*s), 0, NULL);
	}

	system("pause");
	return 0;

}







unsigned int __stdcall mythread(void* data)
{
	char *nodeIdptr = static_cast<char*>(data);
	printf("THREAD [%d]:[%s]:: started.\n", GetCurrentThreadId(), &(*nodeIdptr));
	
	graph::Node *currentNodePtr = _graph.getNode(&(*nodeIdptr));
	//printf("THREAD [%d]:[%s]:: IP = %s\n", GetCurrentThreadId(), &(*nodeIdptr), (currentNodePtr->IP).c_str());

	currentNodePtr->ThreadId = GetCurrentThreadId();
	string VideoLink = currentNodePtr->IP;





	return 0;
}
