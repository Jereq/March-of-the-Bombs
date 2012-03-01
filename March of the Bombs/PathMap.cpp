#include "PathMap.h"

#include <set>
#include <limits>

#include <boost/foreach.hpp>

const int PathMap::nodesPerBlock = 2;

PathMap::CompareNode::CompareNode(PathMap const& pathMap)
	: pathMap(pathMap)
{
}

bool PathMap::CompareNode::operator()(PathNode::nodeIndex const& lhs, PathNode::nodeIndex const& rhs) const
{
	PathNode const& lNode = pathMap.pathArray(lhs);
	PathNode const& rNode = pathMap.pathArray(rhs);

	if (lNode.fScore < rNode.fScore) return true;
	if (lNode.fScore > rNode.fScore) return false;

	return lhs < rhs;
}

void PathMap::resetUseCount() const
{
	for (array_type::size_type i = 0; i < pathArray.shape()[0]; i++)
	{
		for (array_type::size_type j = 0; j < pathArray.shape()[1]; j++)
		{
			pathArray[i][j].useCount = 0;
		}
	}

	useCount = 0;
}

float PathMap::getDistance(PathNode::nodeIndex const& pos1, PathNode::nodeIndex const& pos2) const
{
	return glm::distance(glm::vec2(pos1[0], pos1[1]), glm::vec2(pos2[0], pos2[1])) / nodesPerBlock;
}

PathNode::nodeIndex PathMap::getClosestNode(glm::vec2 const& pos) const
{
	glm::ivec2 res = glm::clamp(glm::ivec2(glm::round(pos * (float)nodesPerBlock - glm::vec2(0.5f))), glm::ivec2(0), glm::ivec2(pathArray.shape()[0] - 1, pathArray.shape()[1] - 1));

	PathNode::nodeIndex resNI = {{res.x, res.y}};
	return resNI;
}

std::list<PathNode::nodeIndex> PathMap::getNonClosedNeighbors(PathNode::nodeIndex const& node) const
{
	const static glm::ivec2 possibleNeighbors[16] =
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

	std::list<PathNode::nodeIndex> res;

	for (int i = 0; i < 16; i++)
	{
		glm::ivec2 const& offset = possibleNeighbors[i];

		PathNode::nodeIndex neighbor = {{node[0] + offset.x, node[1] + offset.y}};

		if (neighbor[0] < 0 ||
			neighbor[0] >= (int)pathArray.shape()[0] ||
			neighbor[1] < 0 ||
			neighbor[1] >= (int)pathArray.shape()[1] ||
			pathArray(neighbor).free == false ||
			pathArray(neighbor).useCount == useCount)
		{
			continue;
		}

		if (node[0] != neighbor[0] &&
			node[1] != neighbor[1] &&
			(!pathArray[node[0]][neighbor[1]].free ||
			!pathArray[neighbor[0]][node[1]].free))
		{
			continue;
		}

		res.push_back(neighbor);
	}

	return res;
}

glm::vec2 PathMap::toVec2(PathNode::nodeIndex const& nodeIndex) const
{
	return glm::vec2(nodeIndex[0] + 0.5f, nodeIndex[1] + 0.5f) / (float)nodesPerBlock;
}

PathMap::PathMap()
	: useCount(0)
{
}

void PathMap::resize(unsigned int width, unsigned int height)
{
	pathArray.resize(boost::extents[width * nodesPerBlock][height * nodesPerBlock]);
}

void PathMap::blockPath(unsigned int x, unsigned int z)
{
	setPathFree(x, z, false);
}

void PathMap::freePath(unsigned int x, unsigned int z)
{
	setPathFree(x, z, true);
}

void PathMap::setPathFree(unsigned int x, unsigned int z, bool free)
{
	for (unsigned int dZ = 0; dZ < nodesPerBlock; dZ++)
	{
		for (unsigned int dX = 0; dX < nodesPerBlock; dX++)
		{
			pathArray[x * nodesPerBlock + dX][z * nodesPerBlock + dZ].free = free;
		}
	}
}

bool PathMap::findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec2>& path) const
{
	if (useCount == std::numeric_limits<unsigned short>::max())
	{
		resetUseCount();
	}
	useCount++;

	path.clear();

	std::set<PathNode::nodeIndex, CompareNode> openSet(CompareNode(*this));

	PathNode::nodeIndex startIdx = getClosestNode(start);
	PathNode::nodeIndex goalIdx = getClosestNode(goal);

	PathNode& startNode = pathArray(startIdx);

	if (!startNode.free ||
		!pathArray(goalIdx).free)
	{
		return false;
	}

	startNode.gScore = 0;
	startNode.hScore = getDistance(startIdx, goalIdx);
	startNode.fScore = startNode.gScore + startNode.hScore;

	openSet.insert(startIdx);

	while (!openSet.empty())
	{
		PathNode::nodeIndex current = *openSet.begin();
		
		if (current == goalIdx)
		{
			break;
		}

		openSet.erase(current);

		pathArray(current).useCount = useCount;

		BOOST_FOREACH(PathNode::nodeIndex const& neighbor, getNonClosedNeighbors(current))
		{
			float tentativeGScore = pathArray(current).gScore + getDistance(current, neighbor);
			bool tentativeIsBetter = false;

			PathNode& neighborNode = pathArray(neighbor);

			if (openSet.count(neighbor) == 0)
			{
				neighborNode.hScore = getDistance(neighbor, goalIdx);
				tentativeIsBetter = true;
			}
			else if (tentativeGScore < neighborNode.gScore)
			{
				tentativeIsBetter = true;
				openSet.erase(neighbor);
			}

			if (tentativeIsBetter)
			{
				neighborNode.prevNode = current;
				neighborNode.gScore = tentativeGScore;
				neighborNode.fScore = neighborNode.gScore + neighborNode.hScore;

				openSet.insert(neighbor);
			}
		}
	}

	if (openSet.empty())
	{
		return false;
	}

	for (PathNode::nodeIndex ind = goalIdx; ind != startIdx; ind = pathArray(ind).prevNode)
	{
		path.push_front(toVec2(ind));
	}

	path.push_front(toVec2(startIdx));

	return true;
}