#pragma once

#include <deque>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>

class Context;

class Player
	: public boost::enable_shared_from_this<Player>
{
private:
	boost::asio::ip::tcp::socket socket;
	boost::shared_ptr<Context> context;
	boost::shared_ptr<PacketManager> packetManager;

	char readBuffer[Packet::MAX_SIZE];
	uint16_t packetLength;
	uint16_t packetId;
	std::deque<Packet::const_ptr> writePackets;

public:
	typedef boost::shared_ptr<Player> ptr;

	Player(boost::asio::io_service& io_service);

	boost::asio::ip::tcp::socket& getSocket();
	void start(boost::shared_ptr<Context> const& context);
	void deliver(Packet::const_ptr packet);

	void handleReadHeader(boost::system::error_code const& error);
	void handleReadBody(boost::system::error_code const& error);
	void handleWrite(boost::system::error_code const& error);
};