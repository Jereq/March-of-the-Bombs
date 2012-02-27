#include "BlockModel.h"

#include "BlockModelData.h"

BlockModel::BlockModel()
	: Model(BlockModelData::getInstance())
{
}