#include <iostream>
#include "Model.h"

using namespace model;

EdgedNode::EdgedNode(){}

EdgedNode::EdgedNode(int _id, int _loc)
{
	NodeId = _id;
	LocInMetrix = _loc;
}

EdgedNode::~EdgedNode(){}