#pragma once
#include "block.h"
#include "Model.h"
#include "MapHeader.h"
#include "HQModelData.h"

class HQBlock :
	public Block
{
private:
	Model::ptr model;
	unsigned short ownerAtTheMax;

public:
	HQBlock(void);
	~HQBlock(void);

	
	void setPosition();
	unsigned short setPlayer(/*player*/);
	int hitPoints;
	void drawHQ(Graphics::ptr graphics);
};

