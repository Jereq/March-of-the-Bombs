#include "Packet14RemoveBlocks.h"

#include "Pack.h"

void Packet14RemoveBlocks::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();
	util::pack(&blocks, 1, &packedData[OFFSET_DATA]);
	
	packed = true;
}

void Packet14RemoveBlocks::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	util::unpack(&blocks, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet14RemoveBlocks::Packet14RemoveBlocks()
	: Packet(mId)
{
}

Packet14RemoveBlocks::Packet14RemoveBlocks(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet14RemoveBlocks::Packet14RemoveBlocks(std::vector<glm::ivec2> const& blocks)
	: Packet(mId), blocks(blocks)
{
	dataLength = OFFSET_DATA +
		util::packedSize(blocks);
	unpacked = true;
}

Packet::ptr Packet14RemoveBlocks::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet14RemoveBlocks(data, length));
}

std::vector<glm::ivec2> const& Packet14RemoveBlocks::getBlocks() const
{
	if (!unpacked)
	{
		unpack();
	}

	return blocks;
}