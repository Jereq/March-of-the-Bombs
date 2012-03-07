#include "Packet7JoinGame.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include "InvalidSizeException.h"

void Packet7JoinGame::pack() const
{
	return;	// If it has data, then it is already packed.
}

void Packet7JoinGame::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if (!checkHeader())
	{
		return;
	}

	gameID = reinterpret_cast<uint16_t*>(packedData + OFFSET_DATA);

	unpacked = true;
}

Packet7JoinGame::Packet7JoinGame()
	: Packet(mId), gameID(NULL)
{
}

Packet7JoinGame::Packet7JoinGame(char const* data, uint16_t length)
	: Packet(mId), gameID(NULL)
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

Packet7JoinGame::Packet7JoinGame(unsigned short gameID)
	: Packet(mId)
{
	dataLength = Packet::MIN_SIZE + sizeof(uint16_t);
	packedData = new char[dataLength];

	this->gameID = reinterpret_cast<uint16_t*>(packedData + OFFSET_DATA);

	packHeader();
	*this->gameID = htons(gameID);

	packed = true;
	unpacked = true;
}

Packet::ptr Packet7JoinGame::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet7JoinGame(data, length));
}

unsigned int Packet7JoinGame::getGameID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return ntohs(*gameID);
}