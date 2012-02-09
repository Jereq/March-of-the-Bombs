#include "Packet.h"

#include <WinSock2.h>

Packet::Packet(uint16_t id)
	: id(id), dataLength(0), packedData(0), packed(false), unpacked(false)
{
}

void Packet::packHeader() const
{
	uint16_t* lengthP = reinterpret_cast<uint16_t*>(&packedData[OFFSET_LENGTH]);
	*lengthP = htons(getDataLength());

	uint16_t* idP = reinterpret_cast<uint16_t*>(&packedData[OFFSET_ID]);
	*idP = htons(getId());
}

bool Packet::checkHeader() const
{
	uint16_t const* lengthP = reinterpret_cast<uint16_t const*>(&packedData[OFFSET_LENGTH]);
	uint16_t length = ntohs(*lengthP);

	if (length != getDataLength())
	{
		return false;
	}

	uint16_t const* idP = reinterpret_cast<uint16_t const*>(&packedData[OFFSET_ID]);
	uint16_t tId = ntohs(*idP);

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
		packedData = 0;
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