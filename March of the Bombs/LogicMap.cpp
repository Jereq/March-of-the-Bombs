#include "MapHeader.h"
#include "EmptyBlock.h"
#include "HardBlock.h"
#include "SoftBlock.h"
#include "HQBlock.h"

void Map::loadDefaultMap()
{
	blockMap.resize(boost::extents[10][10]);
	for(int i = 0; i<10; i++)
		for(int j = 0; j<10; j++)
		{
			switch ((j*i)%4)
			{
			case 0:
				blockMap[i][j]=Block::ptr(new EmptyBlock());
				break;
			case 1:
				blockMap[i][j]=Block::ptr(new SoftBlock());
				break;
			case 2:
				blockMap[i][j]=Block::ptr(new HardBlock());
				break;
			case 3:
				blockMap[i][j]=Block::ptr(new HQBlock());
				break;
			}
		}
}