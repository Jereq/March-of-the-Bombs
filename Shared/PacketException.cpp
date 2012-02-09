#include "PacketException.h"

PacketException::PacketException(std::string const& what, Packet::ptr packet)
	: what(what), packet(packet)
{
}