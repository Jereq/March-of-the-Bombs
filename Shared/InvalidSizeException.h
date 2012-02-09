#pragma once

#include "packetexception.h"

class InvalidSizeException :
	public PacketException
{
public:
	InvalidSizeException(std::string const& what, Packet::ptr packet);
};