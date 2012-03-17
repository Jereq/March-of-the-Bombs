#include "FlagModelData.h"

FlagModelData::ptr FlagModelData::instance;

FlagModelData::FlagModelData()
	: Model3DS("Models/flag.3ds")
{
	if (groups.size() > 0)
	{
		groups[0].texture = GLTexture::getTexture(L"Models/flag.tga", true);
	}
}

FlagModelData::ptr FlagModelData::getInstance()
{
	if (!instance)
	{
		instance = FlagModelData::ptr(new FlagModelData());
	}
	
	return instance;
}