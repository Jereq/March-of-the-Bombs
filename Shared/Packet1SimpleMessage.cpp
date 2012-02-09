#include "Packet1SimpleMessage.h"

#include <WinSock2.h>
#include <iterator>
#include <string>
using std::string;

#include <algorithm>
using std::copy;

void Packet1SimpleMessage::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];
	copy(msg.begin(), msg.end(), stdext::checked_array_iterator<char*>(dataP, length - OFFSET_DATA));

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

	msg = string(&packedData[OFFSET_DATA], getDataLength() - OFFSET_DATA);

	unpacked = true;
}

Packet1SimpleMessage::Packet1SimpleMessage()
	: Packet(mId)
{
}

Packet1SimpleMessage::Packet1SimpleMessage(char const* data, uint16_t length)
	: Packet(mId)
{
	dataLength = length;
	packedData = new char[length];

	copy(data, data + length, stdext::checked_array_iterator<char*>(packedData, length));

	packed = true;
}

Packet1SimpleMessage::Packet1SimpleMessage(string const& msg)
	: Packet(mId), msg(msg)
{
	unpacked = true;
	dataLength = msg.size() + OFFSET_DATA;
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