#include "Packet1SimpleMessage.h"

#include <string>
using std::string;

#include "Pack.h"

void Packet1SimpleMessage::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();
	util::pack(&msg, 1, &packedData[OFFSET_DATA]);

	packed = true;
}

void Packet1SimpleMessage::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	util::unpack(&msg, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet1SimpleMessage::Packet1SimpleMessage()
	: Packet(mId)
{
}

Packet1SimpleMessage::Packet1SimpleMessage(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet1SimpleMessage::Packet1SimpleMessage(string const& msg)
	: Packet(mId), msg(msg)
{
	unpacked = true;
	dataLength = util::packedSize(msg) + OFFSET_DATA;
}

Packet::ptr Packet1SimpleMessage::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet1SimpleMessage(data, length));
}

string const& Packet1SimpleMessage::getMessage() const
{
	if (!unpacked)
	{
		unpack();
	}

	return msg;
}