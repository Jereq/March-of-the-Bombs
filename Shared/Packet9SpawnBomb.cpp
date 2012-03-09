#include "Packet9SpawnBomb.h"

#include "Pack.h"

void Packet9SpawnBomb::pack() const
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

void Packet9SpawnBomb::unpack() const
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

Packet9SpawnBomb::Packet9SpawnBomb()
	: Packet(mId)
{
}

Packet9SpawnBomb::Packet9SpawnBomb(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet9SpawnBomb::Packet9SpawnBomb(unsigned short playerID, unsigned short entityID,
	glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
	: Packet(mId), playerID(playerID), entityID(entityID), position(position), rotation(rotation), velocity(velocity)
{
	dataLength = OFFSET_DATA +
		2 * sizeof(uint16_t) +
		3 * sizeof(glm::vec3);
	unpacked = true;
}

Packet::ptr Packet9SpawnBomb::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet9SpawnBomb(data, length));
}

unsigned short Packet9SpawnBomb::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return playerID;
}

unsigned short Packet9SpawnBomb::getEntityID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return entityID;
}

glm::vec3 const& Packet9SpawnBomb::getPosition() const
{
	if (!unpacked)
	{
		unpack();
	}

	return position;
}

glm::vec3 const& Packet9SpawnBomb::getRotation() const
{
	if (!unpacked)
	{
		unpack();
	}

	return rotation;
}

glm::vec3 const& Packet9SpawnBomb::getVelocity() const
{
	if (!unpacked)
	{
		unpack();
	}

	return velocity;
}