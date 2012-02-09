#pragma once
#pragma comment (lib, "Shared.lib")

#include <map>
#include <boost/asio.hpp>

#include "Packet.h"
#include "PacketException.h"

class PacketManager
{
private:
	std::map<uint16_t, Packet::ptr> packetPrototypes;

public:
	//Packet::ptr receivePacket(boost::asio::ip::tcp::socket& sock) const;
	Packet::ptr createPacket(char const* data, uint16_t length, uint16_t id) const;
	void addPacketPrototype(Packet::ptr packet);
};