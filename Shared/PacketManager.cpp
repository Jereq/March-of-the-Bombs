#include "PacketManager.h"

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <boost/thread.hpp>
using boost::this_thread::interruption_point;

Packet::ptr PacketManager::createPacket(char const* data, uint16_t length, uint16_t id) const
{
	std::map<uint16_t, Packet::ptr>::const_iterator it = packetPrototypes.find(id);
	if (it != packetPrototypes.end())
	{
		return it->second->createPacket(data, length);
	}
	else
	{
		throw PacketException("Received unknown packet id", Packet::ptr());
	}
}

void PacketManager::addPacketPrototype(Packet::ptr packet)
{
	if (packetPrototypes.count(packet->getId()) != 0)
	{
		throw PacketException("Packet id already taken", packet);
	}

	packetPrototypes[packet->getId()] = packet;
}