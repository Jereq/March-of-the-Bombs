#include "Packet12OpenGames.h"

#include "Pack.h"

void Packet12OpenGames::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();
	util::pack(&openGames, 1, &packedData[OFFSET_DATA]);

	packed = true;
}

void Packet12OpenGames::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	util::unpack(&openGames, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet12OpenGames::Packet12OpenGames()
	: Packet(mId)
{
}

Packet12OpenGames::Packet12OpenGames(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet12OpenGames::Packet12OpenGames(std::vector<OpenGame> const& openGames)
	: Packet(mId), openGames(openGames)
{
	dataLength = OFFSET_DATA +
		util::packedSize(openGames);
	unpacked = true;
}

Packet::ptr Packet12OpenGames::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet12OpenGames(data, length));
}

std::vector<OpenGame> const& Packet12OpenGames::getOpenGames() const
{
	if (!unpacked)
	{
		unpack();
	}

	return openGames;
}