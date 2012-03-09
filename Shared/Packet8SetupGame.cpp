#include "Packet8SetupGame.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include <string>
using std::string;

#include "Pack.h"

void Packet8SetupGame::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&opponentID, 1, dataP);
	dataP += util::pack(&opponentColor, 1, dataP);
	dataP += util::pack(&mapName, 1, dataP);
	dataP += util::pack(&baseID, 1, dataP);

	packed = true;
}

void Packet8SetupGame::unpack() const
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

	dataP += util::unpack(&opponentID, 1, dataP);
	dataP += util::unpack(&opponentColor, 1, dataP);
	dataP += util::unpack(&mapName, 1, dataP);
	dataP += util::unpack(&baseID, 1, dataP);

	unpacked = true;
}

Packet8SetupGame::Packet8SetupGame()
	: Packet(mId)
{
}

Packet8SetupGame::Packet8SetupGame(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet8SetupGame::Packet8SetupGame(unsigned short opponentID, glm::vec3 const& opponentColor,
		std::string const& mapName, unsigned short baseID)
	: Packet(mId), opponentID(opponentID), opponentColor(opponentColor), mapName(mapName), baseID(baseID)
{
	dataLength = OFFSET_DATA +
		2 * sizeof(uint16_t) +
		sizeof(glm::vec3) +
		util::packedSize(mapName);

	unpacked = true;
}

Packet::ptr Packet8SetupGame::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet8SetupGame(data, length));
}

unsigned short Packet8SetupGame::getOpponentID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return opponentID;
}
glm::vec3 const& Packet8SetupGame::getOpponentColor() const
{
	if (!unpacked)
	{
		unpack();
	}

	return opponentColor;
}

std::string const& Packet8SetupGame::getMapName() const
{
	if (!unpacked)
	{
		unpack();
	}

	return mapName;
}

unsigned short Packet8SetupGame::getBaseID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return baseID;
}