#pragma once

#include <boost/multi_array.hpp>

#include "Block.h"
#include "PathMap.h"
#include "Graphics.h"

class Map
{
private:
	void updateUnitPositions();
	void updateBuildings();
	void UpdateWalls();

	typedef boost::multi_array<Block::ptr, 2> block2DArray;
	block2DArray blockMap;

	PathMap pathMap;

	Model::ptr groundPlane;

public:
	void loadDefaultMap();
	void loadMapFromFile();

	bool findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec3>& path) const;

	void draw(Graphics::ptr graphics);
};