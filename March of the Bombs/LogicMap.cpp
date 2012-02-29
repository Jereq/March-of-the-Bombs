#include "MapHeader.h"
#include "EmptyBlock.h"
#include "HardBlock.h"
#include "SoftBlock.h"
#include "HQBlock.h"
#include <iostream>
#include <fstream>
using namespace std;

void Map::loadDefaultMap()
{
	const static int size = 40;

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
				pathMap.freePath(i, j);
				break;

			case 1:
				blockMap[i][j] = Block::ptr(new SoftBlock(glm::vec3(i, 0, j)));
				pathMap.blockPath(i, j);
				break;

			case 2:
				blockMap[i][j] = Block::ptr(new HardBlock(glm::vec3(i, 0, j)));
				pathMap.blockPath(i, j);
				break;

			case 3:
				blockMap[i][j] = Block::ptr(new HQBlock());
				pathMap.blockPath(i, j);
				break;
			}
		}
	}
}

void Map::loadMapFromFile()
{
	ofstream mapFile;
	mapFile.open ("defaultmapfile.txt", ios::in);
	if(mapFile.is_open())
		cout << "LOADED!";


	mapFile.close();
}

bool Map::findPath(glm::vec2 const& start, glm::vec2 const& goal, std::list<glm::vec2>& path) const
{
	return pathMap.findPath(start, goal, path);
}

void Map::drawBlocks(Graphics::ptr graphics)
{
	for (unsigned int i = 0; i < blockMap.shape()[0]; i++)
	{
		for (unsigned int j = 0; j < blockMap.shape()[1]; j++)
		{
			blockMap[i][j]->draw(graphics);
		}
	}
}