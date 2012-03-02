#pragma once

#include <list>
#include <boost/multi_array.hpp>
#include <glm/glm.hpp>

#include "PathNode.h"

class PathMap
{
public:
	typedef boost::multi_array<PathNode, 2> array_type;

private:
	class CompareNode
	{
	private:
		PathMap const& pathMap;

	public:
		CompareNode(PathMap const& pathMap);

		bool operator()(PathNode::nodeIndex const& lhs, PathNode::nodeIndex const& rhs) const;
	};

protected:
	const static int nodesPerBlock;

	mutable array_type pathArray;

	mutable unsigned short useCount;

	void resetUseCount() const;
	float getDistance(PathNode::nodeIndex const& pos1, PathNode::nodeIndex const& pos2) const;
	PathNode::nodeIndex getClosestNode(glm::vec2 const& pos) const;
	std::list<PathNode::nodeIndex> getNeighbors(PathNode::nodeIndex const& node) const;

	void updateAllNeighbors(PathNode::nodeIndex const& pos);

	glm::vec2 toVec2(PathNode::nodeIndex const& nodeIndex) const;

public:
	PathMap();

	void resize(unsigned int width, unsigned int height);
	void blockPath(unsigned int x, unsigned int z);
	void freePath(unsigned int x, unsigned int z);
	void setPathFree(unsigned int x, unsigned int z, bool free);

	bool findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec2>& path) const;
};