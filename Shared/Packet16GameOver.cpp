#include "Packet16GameOver.h"

#include "Pack.h"

void Packet16GameOver::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];

	dataP += util::pack(&player1ID, 1, dataP);
	dataP += util::pack(&player2ID, 1, dataP);
	dataP += util::pack(&player1Score, 1, dataP);
	dataP += util::pack(&player2Score, 1, dataP);

	packed = true;
}

void Packet16GameOver::unpack() const
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

	dataP += util::unpack(&player1ID, 1, dataP);
	dataP += util::unpack(&player2ID, 1, dataP);
	dataP += util::unpack(&player1Score, 1, dataP);
	dataP += util::unpack(&player2Score, 1, dataP);

	unpacked = true;
}

Packet16GameOver::Packet16GameOver()
	: Packet(mId)
{
}

Packet16GameOver::Packet16GameOver(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet16GameOver::Packet16GameOver(unsigned short player1ID, unsigned short player2ID,
		float player1Score, float player2Score)
	: Packet(mId), player1ID(player1ID), player2ID(player2ID),
	player1Score(player1Score), player2Score(player2Score)
{
	unpacked = true;
	dataLength = OFFSET_DATA +
		2 * sizeof(uint16_t) +
		2 * sizeof(float);
}

Packet::ptr Packet16GameOver::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet16GameOver(data, length));
}

unsigned short Packet16GameOver::getPlayer1ID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return player1ID;
}

unsigned short Packet16GameOver::getPlayer2ID() const
{
	if (!unpacked)
	{
		unpack();
	}

	return player2ID;
}

float Packet16GameOver::getPlayer1Score() const
{
	if (!unpacked)
	{
		unpack();
	}

	return player1Score;
}

float Packet16GameOver::getPlayer2Score() const
{
	if (!unpacked)
	{
		unpack();
	}

	return player2Score;
}