#include "LifeBars.h"

LifeBars::LifeBars()
{

}

LifeBars::LifeBars(TextureSection BackGroundT, TextureSection LifeBarT, 
			Rectanglef BackGroundR, float depth, bool Lefty,
			glm::vec3 color)
			: BGtexture(BackGroundT), LBtexture(LifeBarT), BGRect(BackGroundR), 
			depth(depth), Lefty(Lefty), color(color)
{
	LBRect.setPosition(BGRect.getPosition());
}

LifeBars::~LifeBars()
{
}

void LifeBars::render(Graphics::ptr graphics)
{
	//backGround
	graphics->drawTexture(BGtexture,BGRect,depth + 0.001f,glm::vec3(1));
	//LifeBar
	graphics->drawTexture(LBtexture,LBRect,depth + 0.002f,color);

}

void LifeBars::updateLB(float PtoW)
{
	if(!Lefty)
	{
		LBRect.setPosition(glm::vec2(BGRect.getPosition().x + (BGRect.getSize().x - (BGRect.getSize().x * PtoW)),BGRect.getPosition().y));
	}
	LBRect.setSize(glm::vec2(BGRect.getSize().x * PtoW, BGRect.getSize().y));
}