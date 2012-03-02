#pragma once

#include <list>
#include <glm/glm.hpp>

#include "PathNode.h"

class PathMap
{
public:
	typedef std::vector<PathNode> array_type;

private:
	class CompareNode
	{
	private:
		PathMap const& pathMap;

	public:
		CompareNode(PathMap const& pathMap);

		bool operator()(PathNode::index_type const& lhs, PathNode::index_type const& rhs) const;
	};

protected:
	const static int nodesPerBlock;

	array_type::size_type width;
	array_type::size_type height;

	mutable array_type pathArray;
	mutable unsigned short useCount;

	// Store between uses to prevent reallocation
	mutable std::vector<PathNode::index_type> openSet;

	void resetUseCount() const;
	float getDistance(PathNode::index_type const& pos1, PathNode::index_type const& pos2) const;
	PathNode::index_type getClosestNode(glm::vec2 const& pos) const;
	std::list<PathNode::index_type> getNeighbors(PathNode::index_type const& node) const;

	void updateAllNeighbors(PathNode::index_type const& pos);

	glm::vec2 toVec2(PathNode::index_type const& nodeIndex) const;
	glm::ivec2 toIVec2(PathNode::index_type const& nodeIndex) const;

public:
	PathMap();

	void resize(size_t width, size_t height);
	void blockPath(size_t x, size_t z);
	void freePath(size_t x, size_t z);
	void setPathFree(size_t x, size_t z, bool free);
	
	void blockPathLazy(size_t x, size_t z);
	void freePathLazy(size_t x, size_t z);
	void setPathFreeLazy(size_t x, size_t z, bool free);
	void calculateNeighbors();

	bool findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec3>& path) const;
};