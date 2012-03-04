#include "MapHeader.h"

#include <iostream>
#include <fstream>
using namespace std;

#include "EmptyBlock.h"
#include "HardBlock.h"
#include "SoftBlock.h"
#include "HQBlock.h"

#include "PlaneModelData.h"

void Map::loadDefaultMap()
{
	const static int size = 100;

	blockMap.resize(boost::extents[size][size]);
	pathMap.resize(size, size);
	
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size; j++)
		{
			switch ((j * i) % 4)
			{
			case 0:
				blockMap[i][j] = Block::ptr(new EmptyBlock());
				pathMap.freePathLazy(i, j);
				break;

			case 1:
				blockMap[i][j] = Block::ptr(new SoftBlock(glm::vec3(i, 0, j)));
				pathMap.blockPathLazy(i, j);
				break;

			case 2:
				blockMap[i][j] = Block::ptr(new HardBlock(glm::vec3(i, 0, j)));
				pathMap.blockPathLazy(i, j);
				break;

			case 3:
				blockMap[i][j] = Block::ptr(new HQBlock());
				pathMap.blockPathLazy(i, j);
				break;
			}
		}
	}

	pathMap.calculateNeighbors();

	groundPlane = Model::ptr(new Model(PlaneModelData::getInstance()));
	groundPlane->setScale(glm::vec3(size));
}

void Map::loadMapFromFile()
{
	ofstream mapFile;
	mapFile.open ("defaultmapfile.txt", ios::in);
	if(mapFile.is_open())
		cout << "LOADED!";


	mapFile.close();
}

bool Map::findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec3>& path) const
{
	return pathMap.findPath(start, goal, path);
}

void Map::draw(Graphics::ptr graphics)
{
	for (unsigned int i = 0; i < blockMap.shape()[0]; i++)
	{
		for (unsigned int j = 0; j < blockMap.shape()[1]; j++)
		{
			blockMap[i][j]->draw(graphics);
		}
	}
	
	graphics->drawModel(groundPlane);
}

bool Map::intersectGround(glm::vec3 const& origin, glm::vec3 const& direction, float& distance) const
{
	return groundPlane->rayIntersect(origin, direction, distance);
}