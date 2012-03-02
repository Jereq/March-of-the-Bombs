#pragma once

#include <list>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>

class PathNode
{
public:
	bool free;
	unsigned short useCount;

	typedef boost::array<boost::multi_array<PathNode, 2>::index, 2> nodeIndex;
	nodeIndex prevNode;

	std::list<PathNode::nodeIndex> neighbors;

	float gScore;
	float hScore;
	float fScore;

	bool isGoal;

	PathNode();
};