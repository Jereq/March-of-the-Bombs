#include "PlaneModel.h"

#include "PlaneModelData.h"

PlaneModel::PlaneModel()
	: Model(PlaneModelData::getInstance())
{
}