#include "Packet15UpdatePlayerScore.h"

#include "Pack.h"

void Packet15UpdatePlayerScore::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&playerID, 1, dataP);
	dataP += util::pack(&newScore, 1, dataP);

	packed = true;
}

void Packet15UpdatePlayerScore::unpack() const
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

	dataP += util::unpack(&playerID, 1, dataP);
	dataP += util::unpack(&newScore, 1, dataP);

	unpacked = true;
}

Packet15UpdatePlayerScore::Packet15UpdatePlayerScore()
	: Packet(mId)
{
}

Packet15UpdatePlayerScore::Packet15UpdatePlayerScore(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet15UpdatePlayerScore::Packet15UpdatePlayerScore(unsigned short playerID, float newScore)
	: Packet(mId), playerID(playerID), newScore(newScore)
{
	dataLength = OFFSET_DATA + 
		sizeof(uint16_t) +
		sizeof(float);
	unpacked = true;
}

Packet::ptr Packet15UpdatePlayerScore::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet15UpdatePlayerScore(data, length));
}

unsigned int Packet15UpdatePlayerScore::getPlayerID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return playerID;
}

float Packet15UpdatePlayerScore::getNewScore() const
{
	if (!unpacked)
	{
		unpack();
	}

	return newScore;
}