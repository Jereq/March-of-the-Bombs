#include "Packet6CreateGame.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include <string>
using std::string;

#include "InvalidSizeException.h"

void Packet6CreateGame::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];
	copy(mapName.begin(), mapName.end(), stdext::checked_array_iterator<char*>(dataP, length - OFFSET_DATA));

	packed = true;
}

void Packet6CreateGame::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	mapName = string(&packedData[OFFSET_DATA], getDataLength() - OFFSET_DATA);

	unpacked = true;
}

Packet6CreateGame::Packet6CreateGame()
	: Packet(mId)
{
}

Packet6CreateGame::Packet6CreateGame(char const* data, uint16_t length)
	: Packet(mId)
{
	packedData = new char[length];
	dataLength = length;

	copy(data, data + length, stdext::checked_array_iterator<char*>(packedData, length));

	packed = true;
}

Packet6CreateGame::Packet6CreateGame(string const& mapName)
	: Packet(mId)
{
	if (mapName.size() > MAX_MAP_NAME_LENGTH)
	{
		throw InvalidSizeException("Name to long", shared_from_this());
	}

	this->mapName = mapName;
	unpacked = true;
	dataLength = mapName.size() + OFFSET_DATA;
}

Packet::ptr Packet6CreateGame::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet6CreateGame(data, length));
}

string const& Packet6CreateGame::getMapName() const
{
	if (!unpacked)
	{
		unpack();
	}

	return mapName;
}