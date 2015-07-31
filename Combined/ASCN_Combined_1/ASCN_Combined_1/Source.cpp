
#include <stdio.h>
#include <iostream>
#include "Graph.h"



int main(int argc, char* argv[])
{
	std::cout << "Advanced Surveillance Camera Network (ASCN)\nInitialting ...\n";

	std::cout << "Network Graph checking ...\n";

	graph::Graph _graph = graph::Graph();

	_graph.initGraph();

	if (!(_graph.isConfigured()))
	{
		// cofigure graph.
	}

	// init graph - initialte threds and assign thred id to nodes in graph
	




	system("pause");
	return 0;

}