#pragma once

#include <list>
#include <glm/glm.hpp>

#include "PathNode.h"

/**
 * A PathMap represents a grid of nodes where a node can be free or blocked. It is used to
 * calculate the shortest path from one location to another on that grid.
 */
class PathMap
{
public:
	typedef std::vector<PathNode> array_type;

private:
	/*
	 * Comparator class for nodes on the PathMap grid.
	 */
	class CompareNode
	{
	private:
		PathMap const& pathMap;

	public:
		/// Constructor.
		CompareNode(PathMap const& pathMap);

		/*
		 * The comparator function for nodes. It compares the nodes' corresponding fScore
		 * and returns true if the left node has a higher score than the right.
		 * In the event of a tie, the nodes indices are compared.
		 */
		bool operator()(PathNode::index_type const& lhs, PathNode::index_type const& rhs) const;
	};

protected:
	/// Resolution of the grid compared to the corresponding block map
	const static int nodesPerBlock;

	array_type::size_type width;
	array_type::size_type height;

	/// The stored nodes, mutable because we update the scores
	mutable array_type pathArray;
	/// counts the number of uses, to save us from having to reset every node between every run
	mutable unsigned short useCount;

	/// Store between uses to prevent reallocation
	mutable std::vector<PathNode::index_type> openSet;

	/// Reset the useCount of the class and of every node
	void resetUseCount() const;
	/// Calculate the distance between two nodes
	float getDistance(PathNode::index_type const& pos1, PathNode::index_type const& pos2) const;
	/// Find the node closest to a position
	PathNode::index_type getClosestNode(glm::vec2 const& pos) const;
	/// Find all neighboring nodes where a node should be connected to
	std::list<PathNode::index_type> getNeighbors(PathNode::index_type const& node) const;

	/// Updates the neighbors of the node specified and all nodes that might be affected by it
	void updateAllNeighbors(PathNode::index_type const& pos);

	/// Calculate the corresponding position in "world" coordinates for a node
	glm::vec2 toVec2(PathNode::index_type const& nodeIndex) const;
	/// Calculate the corresponding position in grid coordinates for a node
	glm::ivec2 toIVec2(PathNode::index_type const& nodeIndex) const;

public:
	/// Constructor.
	PathMap();

	/// Change the size of the path map and reset all nodes
	void resize(size_t width, size_t height);
	/// Mark nodes corresponding to a Block as blocked, and updates all necessary neighbors
	void blockPath(size_t x, size_t z);
	/// Mark nodes corresponding to a Block as free, and updates all necessary neighbors
	void freePath(size_t x, size_t z);
	/// Set nodes corresponding to a Block to the value free, and updates all necessary neighbors
	void setPathFree(size_t x, size_t z, bool free);

	/// Mark nodes corresponding to a Block as blocked, without updating any neighbors
	void blockPathLazy(size_t x, size_t z);
	/// Mark nodes corresponding to a Block as free, without updating any neighbors
	void freePathLazy(size_t x, size_t z);
	/// Set nodes corresponding to a Block to the value free, without updating any neighbors
	void setPathFreeLazy(size_t x, size_t z, bool free);
	/// Calculates neighbors for all nodes
	void calculateNeighbors();

	/**
	 * Calculate a path from start to goal and set the result in path if any found.
	 * 
	 * If a path is found, return true, otherwise return false.
	 * If a path is found, the last value in path will be the value goal.
	 * Any values previously in path are always cleared.
	 * If either start or goal translates to a blocked node, no path is found.
	 */
	bool findPath(glm::vec3 const& start, glm::vec3 const& goal, std::list<glm::vec3>& path) const;
};