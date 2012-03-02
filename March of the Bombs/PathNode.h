#pragma once

#include <list>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>

class PathNode
{
public:
	bool free;
	unsigned short useCount;
	unsigned short openCount;
	
	typedef std::vector<PathNode>::size_type index_type;
	index_type prevNode;

	std::list<PathNode::index_type> neighbors;

	float gScore;
	float hScore;
	float fScore;

	PathNode();
};