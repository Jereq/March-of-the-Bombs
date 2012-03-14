#pragma once
#include "block.h"
//#include "Model.h"
//#include "Map.h"
#include "HQModelData.h"

class HQBlock :
	public Block
{
private:
	unsigned short ownerAtTheMax;

public:
	
	HQBlock(unsigned short owner, glm::vec3 const& position);
	~HQBlock(void);

	
	unsigned short getOwner() const;
	/*void drawHQ(Graphics::ptr graphics);*/

};

