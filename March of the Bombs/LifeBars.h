#pragma once

#include "Rectanglef.h"
#include "TextureSection.h"
#include "Graphics.h"

class LifeBars
{
private:
	bool Lefty;
	float depth;
	Rectanglef BGRect, LBRect;
	TextureSection BGtexture, LBtexture, Edgetexture;

public:
	LifeBars();
	LifeBars(TextureSection BackGroundT, TextureSection LifeBarT, 
			Rectanglef BackGroundR, float depth, bool Lefty);
	~LifeBars();

	void render(Graphics::ptr graphics);
	void updateLB(float PtoW);
};

