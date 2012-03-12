#include "HQModelData.h"


HQModelData::ptr HQModelData::instance;

HQModelData::HQModelData()
	: Model3DS("Models/base.3ds")
{
	if (groups.size() > 0)
	{
		groups[0].texture = GLTexture::getTexture(L"Models/base.png");
	}
}

HQModelData::ptr HQModelData::getInstance()
{
	if (!instance)
	{
		instance = HQModelData::ptr(new HQModelData());
	}

	return instance;
}
