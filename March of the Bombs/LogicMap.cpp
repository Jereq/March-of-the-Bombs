#include "MapHeader.h"
#include "EmptyBlock.h"
#include "HardBlock.h"
#include "SoftBlock.h"
#include "HQBlock.h"
#include "PlaneModelData.h"

#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;
using boost::lexical_cast;
using boost::bad_lexical_cast;

Map::Map(std::string const& fileName)
{
	loadMapFromFile(fileName);
}

void Map::loadDefaultMap()
{
	loadMapFromFile("defaultmapfile.txt");
}

void Map::loadMapFromFile(string c)
{
	string a;
	ifstream mapFile;
	mapFile.open (c, ios::in);
	if(mapFile.is_open())
	{
		std::getline(mapFile, a);
		tokenizer<> tok(a);
		tokenizer<>::iterator beg=tok.begin();
		height = lexical_cast<int>(*beg);
		beg++;
		width = lexical_cast<int>(*beg);

		blockMap.resize(boost::extents[height][width]);
		pathMap.resize(height, width);
		bases.clear();

		for(int k = 0; k < height; k++)
		{
			std::getline(mapFile, a);
			tokenizer<> tok(a);
			tokenizer<>::iterator saz=tok.begin();
			for(int g = 0; g < width; g++)
			{
				int blockSlotType = lexical_cast<int>(*saz);
				switch (blockSlotType)
				{
				case 0:
					blockMap[k][g] = Block::ptr(new EmptyBlock());
					pathMap.freePathLazy(k, g);
					break;

				case 1:
					blockMap[k][g] = Block::ptr(new SoftBlock(glm::vec3(k, 0, g)));
					pathMap.blockPathLazy(k, g);
					break;

				case 2:
					blockMap[k][g] = Block::ptr(new HardBlock(glm::vec3(k, 0, g)));
					pathMap.blockPathLazy(k, g);
					break;

				case 3:
					blockMap[k][g] = Block::ptr(new HQBlock());
					pathMap.freePathLazy(k, g);
					bases.push_back(glm::ivec2(k, g));
					break;
				}
				saz++;
			}
		}
		pathMap.calculateNeighbors();

	groundPlane = Model::ptr(new Model(PlaneModelData::getInstance()));
	groundPlane->setScale(glm::vec3(width, 1, height));
		
	}

	mapFile.close();
}

bool Map::findPath(glm::vec3 const& start, glm::vec3 const& goal, std::list<glm::vec3>& path) const
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

std::vector<glm::ivec2> const& Map::getBases() const
{
	return bases;
}

Block::ptr Map::getBlock(glm::ivec2 const& pos) const
{
	if (pos.x >= 0 && pos.x < height &&
		pos.y >= 0 && pos.y < width)
	{
		return blockMap[pos.x][pos.y];
	}
	else
	{
		return Block::ptr();
	}
}

void Map::removeBlock(glm::ivec2 const& block)
{
	blockMap[block.x][block.y].reset(new EmptyBlock());
	pathMap.freePath(block.x, block.y);
}