#include "Packet5EntityMove.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include <string>
using std::string;

#include "Pack.h"

void Packet5EntityMove::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	util::pack(&playerID, 1, dataP);
	dataP += sizeof(playerID);

	util::pack(&entityID, 1, dataP);
	dataP += sizeof(entityID);

	util::pack(&position, 1, dataP);
	dataP += sizeof(position);

	util::pack(&rotation, 1, dataP);
	dataP += sizeof(rotation);

	util::pack(&velocity, 1, dataP);

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

	util::unpack(&playerID, 1, dataP);
	dataP += sizeof(playerID);

	util::unpack(&entityID, 1, dataP);
	dataP += sizeof(entityID);

	util::unpack(&position, 1, dataP);
	dataP += sizeof(position);

	util::unpack(&rotation, 1, dataP);
	dataP += sizeof(rotation);

	util::unpack(&velocity, 1, dataP);

	unpacked = true;
}

Packet5EntityMove::Packet5EntityMove()
	: Packet(mId)
{
}

Packet5EntityMove::Packet5EntityMove(char const* data, uint16_t length)
	: Packet(mId)
{
	packedData = new char[length];
	dataLength = length;

	copy(data, data + length, stdext::checked_array_iterator<char*>(packedData, length));

	packed = true;
}

Packet5EntityMove::Packet5EntityMove(unsigned short playerID, unsigned short entityID, glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity)
	: Packet(mId), playerID(playerID), entityID(entityID), position(position), rotation(rotation), velocity(velocity)
{
	dataLength = OFFSET_DATA + sizeof(uint16_t) * 2 + sizeof(glm::vec3) * 3;
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