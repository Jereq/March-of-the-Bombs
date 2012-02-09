#pragma once

#include <string>
#include "Packet.h"

class PacketException
{
public:
	const std::string what;
	Packet::ptr packet;

	PacketException(std::string const& what, Packet::ptr packet);
};