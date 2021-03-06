#include "Packet5EntityMove.h"

#include "Pack.h"

void Packet5EntityMove::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&playerID, 1, dataP);
	dataP += util::pack(&entityID, 1, dataP);
	dataP += util::pack(&position, 1, dataP);
	dataP += util::pack(&rotation, 1, dataP);
	dataP += util::pack(&velocity, 1, dataP);

	packed = true;
}

void Packet5EntityMove::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::unpack(&playerID, 1, dataP);
	dataP += util::unpack(&entityID, 1, dataP);
	dataP += util::unpack(&position, 1, dataP);
	dataP += util::unpack(&rotation, 1, dataP);
	dataP += util::unpack(&velocity, 1, dataP);

	unpacked = true;
}

Packet5EntityMove::Packet5EntityMove()
	: Packet(mId)
{
}

Packet5EntityMove::Packet5EntityMove(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet5EntityMove::Packet5EntityMove(unsigned short playerID, unsigned short entityID,
	glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
	: Packet(mId), playerID(playerID), entityID(entityID), position(position), rotation(rotation), velocity(velocity)
{
	dataLength = OFFSET_DATA +
		2 * sizeof(uint16_t) +
		3 * sizeof(glm::vec3);
	unpacked = true;
}

Packet::ptr Packet5EntityMove::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet5EntityMove(data, length));
}

unsigned short Packet5EntityMove::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return playerID;
}

unsigned short Packet5EntityMove::getEntityID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return entityID;
}

glm::vec3 const& Packet5EntityMove::getPosition() const
{
	if (!unpacked)
	{
		unpack();
	}

	return position;
}

glm::vec3 const& Packet5EntityMove::getRotation() const
{
	if (!unpacked)
	{
		unpack();
	}

	return rotation;
}

glm::vec3 const& Packet5EntityMove::getVelocity() const
{
	if (!unpacked)
	{
		unpack();
	}

	return velocity;
}