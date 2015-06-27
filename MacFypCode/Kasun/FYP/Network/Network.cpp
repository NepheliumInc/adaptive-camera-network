// CppTesting.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Model.h"


int main(int argc, char* argv[])
{
	model::Graph g;
	model::Node nodeFrontDoor = model::Node(123, "start", "192.168.8.27", "Font Door");
	bool x = g.addNode(nodeFrontDoor);
	std::vector<model::Node> y = g.getAllNodes();
	model::Node z = g.getNode(123);


	// end of program
	std::cout << "done..." << std::endl;
	std::string str;
	std::getline(std::cin, str);
	return 0;
}

