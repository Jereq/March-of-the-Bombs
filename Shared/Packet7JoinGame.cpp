#include "Packet7JoinGame.h"

#include "Pack.h"
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
	: Packet(mId, data, length), gameID(NULL)
{
}

Packet7JoinGame::Packet7JoinGame(unsigned short gameID)
	: Packet(mId)
{
	dataLength = sizeof(uint16_t) + OFFSET_DATA;
	packedData = new char[dataLength];

	this->gameID = reinterpret_cast<uint16_t*>(&packedData[OFFSET_DATA]);

	packHeader();
	uint16_t tId = gameID;
	util::pack(&tId, 1, &packedData[OFFSET_DATA]);

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