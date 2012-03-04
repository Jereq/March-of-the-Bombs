#include "PathMap.h"

#include <set>
#include <limits>

#include <boost/foreach.hpp>

const int PathMap::nodesPerBlock = 2;

PathMap::CompareNode::CompareNode(PathMap const& pathMap)
	: pathMap(pathMap)
{
}

bool PathMap::CompareNode::operator()(PathNode::index_type const& lhs, PathNode::index_type const& rhs) const
{
	PathNode const& lNode = pathMap.pathArray[lhs];
	PathNode const& rNode = pathMap.pathArray[rhs];

	if (lNode.fScore > rNode.fScore) return true;
	if (lNode.fScore < rNode.fScore) return false;

	return lhs > rhs;
}

void PathMap::resetUseCount() const
{
	BOOST_FOREACH(PathNode& node, pathArray)
	{
		node.useCount = 0;
		node.openCount = 0;
	}

	useCount = 0;
}

float PathMap::getDistance(PathNode::index_type const& pos1, PathNode::index_type const& pos2) const
{
	return glm::distance(toVec2(pos1), toVec2(pos2)) / nodesPerBlock;
}

PathNode::index_type PathMap::getClosestNode(glm::vec2 const& pos) const
{
	glm::ivec2 res = glm::clamp(glm::ivec2(glm::round(pos * (float)nodesPerBlock - glm::vec2(0.5f))), glm::ivec2(0), glm::ivec2(width - 1, height - 1));

	return res.y * width + res.x;
}

std::list<PathNode::index_type> PathMap::getNeighbors(PathNode::index_type const& node) const
{
	std::list<PathNode::index_type> res;

	if (!pathArray[node].free)
	{
		return res;
	}

	const static int numNeighbors = 16;
	const static glm::ivec2 possibleNeighbors[numNeighbors] =
	{
		glm::ivec2(-2, -1),
		glm::ivec2(-2,  1),
		glm::ivec2(-1, -2),
		glm::ivec2(-1, -1),
		glm::ivec2(-1,  0),
		glm::ivec2(-1,  1),
		glm::ivec2(-1,  2),
		glm::ivec2( 0, -1),
		glm::ivec2( 0,  1),
		glm::ivec2( 1, -2),
		glm::ivec2( 1, -1),
		glm::ivec2( 1,  0),
		glm::ivec2( 1,  1),
		glm::ivec2( 1,  2),
		glm::ivec2( 2, -1),
		glm::ivec2( 2,  1)
	};

	for (int i = 0; i < numNeighbors; i++)
	{
		glm::ivec2 iNode = toIVec2(node);
		glm::ivec2 iNeighbor = iNode + possibleNeighbors[i];

		PathNode::index_type neighbor = iNeighbor.y * width + iNeighbor.x;

		if (iNeighbor.x < 0 ||
			iNeighbor.x >= (int)width ||
			iNeighbor.y < 0 ||
			iNeighbor.y >= (int)height ||
			pathArray[neighbor].free == false)
		{
			continue;
		}

		if (iNode.x == iNeighbor.x ||
			iNode.y == iNeighbor.y ||
			(pathArray[iNode.y * width + iNeighbor.x].free &&
			pathArray[iNeighbor.y * width + iNode.x].free))
		{
			res.push_back(neighbor);
		}
	}

	return res;
}

void PathMap::updateAllNeighbors(PathNode::index_type const& pos)
{
	const static int numNeighbors = 21;
	const static glm::ivec2 neighborOffsets[numNeighbors] =
	{
		glm::ivec2(-1, -2),
		glm::ivec2( 0, -2),
		glm::ivec2( 1, -2),
		glm::ivec2(-2, -1),
		glm::ivec2(-1, -1),
		glm::ivec2( 0, -1),
		glm::ivec2( 1, -1),
		glm::ivec2( 2, -1),
		glm::ivec2(-2,  0),
		glm::ivec2(-1,  0),
		glm::ivec2( 0,  0),
		glm::ivec2( 1,  0),
		glm::ivec2( 2,  0),
		glm::ivec2(-2,  1),
		glm::ivec2(-1,  1),
		glm::ivec2( 0,  1),
		glm::ivec2( 1,  1),
		glm::ivec2( 2,  1),
		glm::ivec2(-1,  2),
		glm::ivec2( 0,  2),
		glm::ivec2( 1,  2) 
	};

	glm::ivec2 iPos = toIVec2(pos);

	for (int i = 0; i < numNeighbors; i++)
	{
		glm::ivec2 iNeighbor = iPos + neighborOffsets[i];
		PathNode::index_type neighbor = iNeighbor.y * width + iNeighbor.x;

		if (iNeighbor.x < 0 ||
			iNeighbor.x >= (int)width ||
			iNeighbor.y < 0 ||
			iNeighbor.y >= (int)height)
		{
			continue;
		}

		PathNode& neighborNode = pathArray[neighbor];
		neighborNode.neighbors = getNeighbors(neighbor);
	}
}

glm::vec2 PathMap::toVec2(PathNode::index_type const& index) const
{
	return glm::vec2((index % width) + 0.5f, (index / width) + 0.5f) / (float)nodesPerBlock;
}

glm::ivec2 PathMap::toIVec2(PathNode::index_type const& index) const
{
	return glm::ivec2(index % width, index / width);
}

PathMap::PathMap()
	: useCount(0), width(0), height(0)
{
	openSet.reserve(100);
}

void PathMap::resize(size_t width, size_t height)
{
	this->width = width * nodesPerBlock;
	this->height = height * nodesPerBlock;

	pathArray.clear();
	pathArray.resize(this->width * this->height);
}

void PathMap::blockPath(size_t x, size_t z)
{
	setPathFree(x, z, false);
}

void PathMap::freePath(size_t x, size_t z)
{
	setPathFree(x, z, true);
}

void PathMap::setPathFree(size_t x, size_t z, bool free)
{
	for (unsigned int dZ = 0; dZ < nodesPerBlock; dZ++)
	{
		size_t row = z * nodesPerBlock + dZ;

		for (unsigned int dX = 0; dX < nodesPerBlock; dX++)
		{
			size_t column = x * nodesPerBlock + dX;
			PathNode::index_type index = row * width + column;

			PathNode& node = pathArray[index];
			if (node.free != free)
			{
				node.free = free;
				updateAllNeighbors(index);
			}
		}
	}
}

void PathMap::blockPathLazy(size_t x, size_t z)
{
	setPathFreeLazy(x, z, false);
}

void PathMap::freePathLazy(size_t x, size_t z)
{
	setPathFreeLazy(x, z, true);
}

void PathMap::setPathFreeLazy(size_t x, size_t z, bool free)
{
	for (unsigned int dZ = 0; dZ < nodesPerBlock; dZ++)
	{
		size_t row = z * nodesPerBlock + dZ;

		for (unsigned int dX = 0; dX < nodesPerBlock; dX++)
		{
			size_t column = x * nodesPerBlock + dX;
			PathNode::index_type index = row * width + column;

			pathArray[index].free = free;
		}
	}
}

void PathMap::calculateNeighbors()
{
	for (PathNode::index_type index = 0; index < pathArray.size(); index++)
	{
		pathArray[index].neighbors = getNeighbors(index);
	}
}

template<class iter, class baseType, class compare>
void move_forward_heap(iter root, iter end, baseType const& target, compare const& comparator)
{
	std::push_heap(root, std::find(root, end, target) + 1, comparator);
}

bool PathMap::findPath(glm::vec3 const& start, glm::vec3 const& goal, std::list<glm::vec3>& path) const
{
	if (useCount == std::numeric_limits<unsigned short>::max())
	{
		resetUseCount();
	}
	useCount++;

	path.clear();

	PathNode::index_type startIdx = getClosestNode(start.swizzle(glm::X, glm::Z));
	PathNode::index_type goalIdx = getClosestNode(goal.swizzle(glm::X, glm::Z));

	PathNode& startNode = pathArray[startIdx];
	PathNode& goalNode = pathArray[goalIdx];

	if (!startNode.free ||
		!goalNode.free)
	{
		return false;
	}

	startNode.gScore = 0;
	startNode.hScore = getDistance(startIdx, goalIdx);
	startNode.fScore = startNode.gScore + startNode.hScore;

	CompareNode compare(*this);

	openSet.clear();
	openSet.push_back(startIdx);
	startNode.openCount = useCount;

	while (!openSet.empty())
	{
		PathNode::index_type current = openSet.front();
		
		if (current == goalIdx)
		{
			break;
		}

		startNode.openCount = 0;
		move_forward_heap(openSet.begin(), openSet.end(), current, compare);
		std::pop_heap(openSet.begin(), openSet.end(), compare);
		openSet.pop_back();

		pathArray[current].useCount = useCount;

		BOOST_FOREACH(PathNode::index_type const& neighbor, pathArray[current].neighbors)
		{
			PathNode& neighborNode = pathArray[neighbor];

			if (neighborNode.useCount == useCount)
			{
				continue;
			}

			float tentativeGScore = pathArray[current].gScore + getDistance(current, neighbor);
			bool tentativeIsBetter = false;
			bool insert = false;
			bool move = false;

			if (neighborNode.openCount != useCount)
			{
				neighborNode.hScore = getDistance(neighbor, goalIdx);
				tentativeIsBetter = true;
				insert = true;
			}
			else if (tentativeGScore < neighborNode.gScore)
			{
				tentativeIsBetter = true;
				neighborNode.openCount = 0;
				move = true;
			}

			if (tentativeIsBetter)
			{
				neighborNode.prevNode = current;
				neighborNode.gScore = tentativeGScore;
				neighborNode.fScore = neighborNode.gScore + neighborNode.hScore;

				neighborNode.openCount = useCount;
			}

			if (insert)
			{
				openSet.push_back(neighbor);
				std::push_heap(openSet.begin(), openSet.end(), compare);
			}
			else if (move)
			{
				move_forward_heap(openSet.begin(), openSet.end(), neighbor, compare);
			}
		}
	}

	if (openSet.empty())
	{
		return false;
	}

	for (PathNode::index_type ind = goalIdx; ind != startIdx; ind = pathArray[ind].prevNode)
	{
		glm::vec2 res = toVec2(ind);
		path.push_front(glm::vec3(res.x, 0, res.y));
	}

	glm::vec2 res = toVec2(startIdx);
	path.push_front(glm::vec3(res.x, 0, res.y));

	return true;
}