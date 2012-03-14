#include "Map.h"
#include "EmptyBlock.h"
#include "HardBlock.h"
#include "SoftBlock.h"
#include "HQBlock.h"
#include "Flag.h"
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
	loadMapFromFile("Maps/defaultmapfile.txt");
}

void Map::loadMapFromFile(string const& c)
{
	string a;
	ifstream mapFile;
	mapFile.open (c, ios::in);
	if(mapFile.is_open())
	{
		std::getline(mapFile, a);
		tokenizer<> tok(a);
		tokenizer<>::iterator beg=tok.begin();
		size.x = lexical_cast<int>(*beg);
		beg++;
		size.y = lexical_cast<int>(*beg);

		blockMap.resize(boost::extents[size.x][size.y]);
		pathMap.resize(size.x, size.y);

		numChunks.x = (size.x - 1) / CHUNK_SIZE + 1;
		numChunks.y = (size.y - 1) / CHUNK_SIZE + 1;
		bombsPerChunk.resize(numChunks.x * numChunks.y);

		bases.clear();

		for(int k = 0; k < size.y; k++)
		{
			std::getline(mapFile, a);
			tokenizer<> tok(a);
			tokenizer<>::iterator saz=tok.begin();
			for(int g = 0; g < size.x; g++)
			{
				unsigned short playerAssignment = 1;
				int blockSlotType = lexical_cast<int>(*saz);
				switch (blockSlotType)
				{
				case 0:
					blockMap[g][k] = Block::ptr(new EmptyBlock());
					pathMap.freePathLazy(g, k);
					break;

				case 1:
					blockMap[g][k] = Block::ptr(new SoftBlock(glm::vec3(g, 0, k)));
					pathMap.blockPathLazy(g, k);
					break;

				case 2:
					blockMap[g][k] = Block::ptr(new HardBlock(glm::vec3(g, 0, k)));
					pathMap.blockPathLazy(g, k);
					break;

				case 3:
					blockMap[g][k] = Block::ptr(new HQBlock(playerAssignment, glm::vec3(g, 0, k)));
					pathMap.freePathLazy(g, k);
					bases.push_back(glm::ivec2(g, k));
					playerAssignment++;
					break;
				
				case 4:
					blockMap[g][k] = Block::ptr(new Flag(glm::vec3(g, 0, k)));
					pathMap.freePathLazy(g, k);
					flagPos = glm::ivec2(g, k);
				}
				saz++;
			}
		}
		pathMap.calculateNeighbors();

		groundPlane = Model::ptr(new Model(PlaneModelData::getInstance()));
		groundPlane->setScale(glm::vec3(size.x, 1, size.y));

	}

	mapFile.close();
}

size_t Map::toChunkNum(glm::ivec2 const& block) const
{
	int x = block.x / CHUNK_SIZE;
	int y = block.y / CHUNK_SIZE;

	return x + numChunks.x * y;
}

bool Map::isValidBlock(glm::ivec2 const& block) const
{
	return block.x >= 0 && block.x < size.x &&
		block.y >= 0 && block.y < size.y;
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
	if (pos.x >= 0 && pos.x < size.x &&
		pos.y >= 0 && pos.y < size.y)
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

glm::ivec2 Map::getSize() const
{
	return size;
}

void Map::getNearbyBombs(glm::vec2 const& center, glm::vec2 const& distance, Bomb::id_set& res) const
{
	glm::ivec2 minPos(glm::floor(center - distance));
	glm::ivec2 maxPos(glm::ceil(center + distance));

	glm::ivec2 minChunk = minPos / (int)CHUNK_SIZE;
	glm::ivec2 maxChunk = maxPos / (int)CHUNK_SIZE;

	minChunk = glm::clamp(minChunk, glm::ivec2(0, 0), numChunks - glm::ivec2(1));
	maxChunk = glm::clamp(maxChunk, glm::ivec2(0, 0), numChunks - glm::ivec2(1));

	for (int x = minChunk.x; x <= maxChunk.x; x++)
	{
		for (int y = minChunk.y; y <= maxChunk.y; y++)
		{
			Bomb::id_set const& chunk = bombsPerChunk[x + numChunks.x * y];
			res.insert(chunk.begin(), chunk.end());
		}
	}
}

void Map::addBombToChunk(glm::ivec2 const& block, Bomb::id const& bombID)
{
	if (!isValidBlock(block))
	{
		return;
	}

	bombsPerChunk[toChunkNum(block)].insert(bombID);
}

void Map::removeBombFromChunk(glm::ivec2 const& block, Bomb::id const& bombID)
{
	if (!isValidBlock(block))
	{
		return;
	}

	bombsPerChunk[toChunkNum(block)].erase(bombID);
}