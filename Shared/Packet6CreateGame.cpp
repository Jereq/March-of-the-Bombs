#include "Packet6CreateGame.h"

#include <string>
using std::string;

#include "Pack.h"
#include "InvalidSizeException.h"

void Packet6CreateGame::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();
	util::pack(&mapName, 1, &packedData[OFFSET_DATA]);

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

	util::unpack(&mapName, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet6CreateGame::Packet6CreateGame()
	: Packet(mId)
{
}

Packet6CreateGame::Packet6CreateGame(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet6CreateGame::Packet6CreateGame(string const& mapName)
	: Packet(mId), mapName(mapName)
{
	if (mapName.size() > MAX_MAP_NAME_LENGTH)
	{
		throw InvalidSizeException("Name to long", Packet::ptr());
	}

	unpacked = true;
	dataLength = util::packedSize(mapName) + OFFSET_DATA;
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