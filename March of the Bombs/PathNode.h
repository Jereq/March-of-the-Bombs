#pragma once

#include <list>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>

/*
 * A PathNode represents a node in a PathMap, used to find the shortest path between two points.
 */
class PathNode
{
public:
	/// Only a free node can be traveled
	bool free;
	/// If useCount is equal to PathMap::useCount, then the node is closed
	unsigned short useCount;
	/// If openCount is equal to PathMap::useCount, then the node is open
	unsigned short openCount;
	
	typedef std::vector<PathNode>::size_type index_type;
	/// The node which leads back toward the start node
	index_type prevNode;

	/// The nodes this node can travel to
	std::list<PathNode::index_type> neighbors;

	/// Path-cost, the distance along a path from the start node to this one
	float gScore;
	/// Admissible "heuristic estimate" of the distance to the goal
	float hScore;
	/// The sum of gScore and hScore
	float fScore;

	/// Constructor.
	PathNode();
};