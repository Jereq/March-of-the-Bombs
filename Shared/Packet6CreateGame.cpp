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

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&mapName, 1, dataP);
	dataP += util::pack(&winLimit, 1, dataP);

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

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::unpack(&mapName, 1, dataP);
	dataP += util::unpack(&winLimit, 1, dataP);

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

Packet6CreateGame::Packet6CreateGame(string const& mapName, unsigned short winLimit)
	: Packet(mId), mapName(mapName), winLimit(winLimit)
{
	if (mapName.size() > MAX_MAP_NAME_LENGTH)
	{
		throw InvalidSizeException("Name to long", Packet::ptr());
	}

	unpacked = true;

	dataLength = OFFSET_DATA + 
		util::packedSize(mapName) +
		sizeof(uint16_t);
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

unsigned short Packet6CreateGame::getWinLimit() const
{
	if (!unpacked)
	{
		unpack();
	}

	return winLimit;
}