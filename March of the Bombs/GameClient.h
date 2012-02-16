#pragma once

#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <Packet.h>
#include <PacketManager.h>

class GameClient
{
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::socket socket;
	boost::thread thread;

	PacketManager packetManager;

	char readBuffer[Packet::MAX_SIZE];
	uint16_t packetLength;
	uint16_t packetId;
	std::deque<Packet::ptr> writePackets;
	std::deque<Packet::ptr> receivedPackets;

	void registerPackets();

	void handleConnect(boost::system::error_code const& error);
	void handleReadHeader(boost::system::error_code const& error);
	void handleReadBody(boost::system::error_code const& error);
	void handleWrite(boost::system::error_code const& error);
	void doWrite(Packet::ptr packet);
	void doClose();
	void startWrite();
	void startRead();

public:
	typedef boost::shared_ptr<GameClient> ptr;

	GameClient(boost::asio::io_service& io_service,
		boost::asio::ip::tcp::resolver::iterator endpointIterator);
	~GameClient();

	void start();

	void write(Packet::ptr packet);
	void close();

	bool hasReceivedPackets() const;
	Packet::ptr popReceivedPacket();

	bool isRunning();
};