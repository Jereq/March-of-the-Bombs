#include "Packet.h"

#include <WinSock2.h>
#include <algorithm>

#include "Pack.h"

Packet::Packet(uint16_t id)
	: id(id), dataLength(0), packedData(0), packed(false), unpacked(false)
{
}

Packet::Packet(uint16_t id, char const* data, uint16_t length)
	: id(id), dataLength(length), packedData(new char[length]), packed(true), unpacked(false)
{
	std::copy(data, data + length, stdext::checked_array_iterator<char*>(packedData, length));
}

void Packet::packHeader() const
{
	uint16_t dataLength = getDataLength();
	util::pack(&dataLength, 1, &packedData[OFFSET_LENGTH]);

	uint16_t id = getId();
	util::pack(&id, 1, &packedData[OFFSET_ID]);
}

bool Packet::checkHeader() const
{
	uint16_t length;
	util::unpack(&length, 1, &packedData[OFFSET_LENGTH]);

	if (length != getDataLength())
	{
		return false;
	}

	uint16_t tId;
	util::unpack(&tId, 1, &packedData[OFFSET_ID]);

	if (tId != getId())
	{
		return false;
	}

	return true;
}

Packet::~Packet()
{
	if (packedData)
	{
		delete[] packedData;
		packedData = NULL;
	}
}

char const* Packet::getData() const
{
	if (!packed)
	{
		pack();
	}

	return packedData;
}

uint16_t Packet::getId() const
{
	return id;
}

uint16_t Packet::getDataLength() const
{
	return dataLength;
}