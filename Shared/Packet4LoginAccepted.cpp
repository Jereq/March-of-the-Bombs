#include "Packet4LoginAccepted.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include "InvalidSizeException.h"

void Packet4LoginAccepted::pack() const
{
	return;	// If it has data, then it is already packed.
}

void Packet4LoginAccepted::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if (!checkHeader())
	{
		return;
	}

	playerID = reinterpret_cast<uint16_t*>(packedData + OFFSET_DATA);

	unpacked = true;
}

Packet4LoginAccepted::Packet4LoginAccepted()
	: Packet(mId), playerID(NULL)
{
}

Packet4LoginAccepted::Packet4LoginAccepted(char const* data, uint16_t length)
	: Packet(mId), playerID(NULL)
{
	uint16_t const* lengthP = reinterpret_cast<uint16_t const*>(&data[OFFSET_LENGTH]);
	uint16_t totalLength = ntohs(*lengthP);

	if (totalLength < MIN_SIZE)
	{
		return;
	}

	uint16_t const* idP = reinterpret_cast<uint16_t const*>(&data[OFFSET_ID]);
	
	if(id != ntohs(*idP))
	{
		return;
	}

	packedData = new char[totalLength];
	copy(data, data + totalLength, stdext::checked_array_iterator<char*>(packedData, totalLength));

	dataLength = totalLength;

	packed = true;
}

Packet4LoginAccepted::Packet4LoginAccepted(unsigned int playerID)
	: Packet(mId)
{
	if (playerID > UINT16_MAX)
	{
		throw InvalidSizeException("Player ID to great", shared_from_this());
	}

	dataLength = Packet::MIN_SIZE + sizeof(uint16_t);
	packedData = new char[dataLength];

	this->playerID = reinterpret_cast<uint16_t*>(packedData + OFFSET_DATA);

	packHeader();
	*this->playerID = htons(playerID);

	packed = true;
	unpacked = true;
}

Packet::ptr Packet4LoginAccepted::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet4LoginAccepted(data, length));
}

unsigned int Packet4LoginAccepted::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return ntohs(*playerID);
}