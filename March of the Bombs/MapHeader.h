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
	std::vector<glm::ivec2> bases;
	PathMap pathMap;

	int height;
	int width;

	Model::ptr groundPlane;

public:
	Map(std::string const& fileName);

	void loadDefaultMap();
	void loadMapFromFile(string);

	bool findPath(glm::vec3 const& start, glm::vec3 const& goal, std::list<glm::vec3>& path) const;

	void draw(Graphics::ptr graphics);

	bool intersectGround(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const;

	std::vector<glm::ivec2> const& getBases() const;
};