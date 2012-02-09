#include "InvalidSizeException.h"

InvalidSizeException::InvalidSizeException(std::string const& what, Packet::ptr packet)
	: PacketException(what, packet)
{
}