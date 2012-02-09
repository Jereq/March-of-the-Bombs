#include "PacketManager.h"

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <boost/thread.hpp>
using boost::this_thread::interruption_point;

//Packet::ptr PacketManager::receivePacket(boost::asio::ip::tcp::socket& sock) const
//{
//	const int BUFFER_LEN = Packet::MAX_SIZE * 2;
//	char buffer[BUFFER_LEN];
//
//	int receivedLength = 0;
//	char* bufferPos = buffer;
//	int remainingBufferLen = BUFFER_LEN;
//
//	// Receive at least the packet size
//	while (receivedLength < sizeof(uint16_t))
//	{
//		int res = receiveOnce(sock, bufferPos, remainingBufferLen);
//		receivedLength += res;
//		bufferPos += res;
//		remainingBufferLen -= res;
//	}
//
//	uint16_t* lengthP = reinterpret_cast<uint16_t*>(buffer);
//	uint16_t lengthToReceive = ntohs(*lengthP);
//
//	if (lengthToReceive < Packet::MIN_SIZE)
//	{
//		throw SockException("Invalid packet length: packet to small");
//	}
//
//	// Receive at least the packet id
//	while (receivedLength < Packet::MIN_SIZE)
//	{
//		int res = receiveOnce(sock, bufferPos, remainingBufferLen);
//		receivedLength += res;
//		bufferPos += res;
//		remainingBufferLen -= res;
//	}
//
//	uint16_t* idP = reinterpret_cast<uint16_t*>(&buffer[2]);
//	uint16_t id = ntohs(*idP);
//
//	if (packetPrototypes.count(id) == 0)
//	{
//		ostringstream oStr;
//		oStr << "Unkown packet id: " << id;
//		throw SockException(oStr.str());
//	}
//
//	// Receive at least one packet
//	while (receivedLength < lengthToReceive)
//	{
//		int res = receiveOnce(sock, bufferPos, remainingBufferLen);
//		receivedLength += res;
//		bufferPos += res;
//		remainingBufferLen -= res;
//	}
//
//	if (receivedLength == lengthToReceive)
//	{
//		return createPacket(buffer, lengthToReceive, id);
//	}
//
//	// TODO: Handle receiving less or more than one packet
//	else if (receivedLength > lengthToReceive)
//	{
//		throw SockException("Received more than one packet");
//	}
//	else
//	{
//		throw SockException("Received less than one packet");
//	}
//}

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