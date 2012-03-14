#pragma once
#include "block.h"
#include "Model.h"
#include "Map.h"
#include "HQModelData.h"

class HQBlock :
	public Block
{
private:
	unsigned short ownerAtTheMax;

public:
	
	HQBlock(unsigned short owner);
	~HQBlock(void);

	
	unsigned short getOwner() const;
	void setPosition();
	unsigned short setPlayer(/*player*/);
	int hitPoints;
	/*void drawHQ(Graphics::ptr graphics);*/

};

