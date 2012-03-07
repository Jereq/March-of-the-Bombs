#include "Packet3Login.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

#include <string>
using std::string;

#include "InvalidSizeException.h"

void Packet3Login::pack() const
{
	uint16_t length = getDataLength();

	if (!packedData)
	{
		packedData = new char[length];
	}

	packHeader();

	char* dataP = &packedData[OFFSET_DATA];
	copy(name.begin(), name.end(), stdext::checked_array_iterator<char*>(dataP, length - OFFSET_DATA));

	packed = true;
}

void Packet3Login::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if(!checkHeader())
	{
		return;
	}

	name = string(&packedData[OFFSET_DATA], getDataLength() - OFFSET_DATA);

	unpacked = true;
}

Packet3Login::Packet3Login()
	: Packet(mId)
{
}

Packet3Login::Packet3Login(char const* data, uint16_t length)
	: Packet(mId)
{
	packedData = new char[length];
	dataLength = length;

	copy(data, data + length, stdext::checked_array_iterator<char*>(packedData, length));

	packed = true;
}

Packet3Login::Packet3Login(string const& name)
	: Packet(mId)
{
	if (name.size() > MAX_NAME_LENGTH)
	{
		throw InvalidSizeException("Name to long", shared_from_this());
	}

	this->name = name;
	unpacked = true;
	dataLength = name.size() + OFFSET_DATA;
}

Packet::ptr Packet3Login::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet3Login(data, length));
}

string const& Packet3Login::getName() const
{
	if (!unpacked)
	{
		unpack();
	}

	return name;
}