#include "Packet3Login.h"

#include <string>
using std::string;

#include "Pack.h"

#include "InvalidSizeException.h"

void Packet3Login::pack() const
{
	if (!packedData)
	{
		packedData = new char[getDataLength()];
	}

	packHeader();
	util::pack(&name, 1, &packedData[OFFSET_DATA]);

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

	util::unpack(&name, 1, &packedData[OFFSET_DATA]);

	unpacked = true;
}

Packet3Login::Packet3Login()
	: Packet(mId)
{
}

Packet3Login::Packet3Login(char const* data, uint16_t length)
	: Packet(mId, data, length)
{
}

Packet3Login::Packet3Login(string const& name)
	: Packet(mId)
{
	if (name.size() > MAX_NAME_LENGTH)
	{
		throw InvalidSizeException("Name to long", Packet::ptr());
	}

	this->name = name;
	unpacked = true;
	dataLength = util::packedSize(name) + OFFSET_DATA;
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