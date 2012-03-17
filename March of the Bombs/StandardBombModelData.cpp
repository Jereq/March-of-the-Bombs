#include "StandardBombModelData.h"

StandardBombModelData::ptr StandardBombModelData::instance;

StandardBombModelData::StandardBombModelData()
	: Model3DS("Models/Standard.3ds")
{
	if (groups.size() > 0)
	{
		groups[0].texture = GLTexture::getTexture(L"Models/Standard.png", true);
	}
}

StandardBombModelData::ptr StandardBombModelData::getInstance()
{
	if (!instance)
	{
		instance = StandardBombModelData::ptr(new StandardBombModelData());
	}

	return instance;
}