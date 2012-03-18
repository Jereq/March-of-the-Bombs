#include "LifeBars.h"

LifeBars::LifeBars()
{

}

LifeBars::LifeBars(TextureSection BackGroundT, TextureSection LifeBarT, 
			Rectanglef BackGroundR, float depth, bool Lefty)
			: BGtexture(BackGroundT), LBtexture(LifeBarT), BGRect(BackGroundR), depth(depth), Lefty(Lefty)
{
	LBRect.setPosition(BGRect.getPosition());
}

LifeBars::~LifeBars()
{
}

void LifeBars::render(Graphics::ptr graphics)
{
	//backGround
	graphics->drawTexture(BGtexture,BGRect,depth + 0.01f,glm::vec3(1));
	//LifeBar
	graphics->drawTexture(LBtexture,LBRect,depth + 0.001f,glm::vec3(1));

}

void LifeBars::updateLB(float PtoW)
{
	if(!Lefty)
	{
		LBRect.setPosition(glm::vec2((BGRect.getSize().x - (BGRect.getSize().x * PtoW)),BGRect.getSize().y));
	}
	LBRect.setSize(glm::vec2(BGRect.getSize().x * PtoW, BGRect.getSize().y));
}