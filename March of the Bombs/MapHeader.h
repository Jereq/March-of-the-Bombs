#pragma once
#include <boost/multi_array.hpp>
#include "Block.h"


class Map
{
private:
	void loadDefaultMap();
	void updateUnitPositions();
	void updateBuildings();
	void UpdateWalls();

	typedef boost::multi_array<Block::ptr, 2> block2DArray;
	block2DArray blockMap;
};