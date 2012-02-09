#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>

#include "player.h"
#include "Game.h"

class PlayerSession
	: public Player,
	public boost::enable_shared_from_this<PlayerSession>
{
private:
	boost::asio::ip::tcp::socket socket;
	Game& game;
	PacketManager& packetManager;

	char readBuffer[Packet::MAX_SIZE];
	uint16_t packetLength;
	uint16_t packetId;
	std::deque<Packet::const_ptr> writePackets;

public:
	typedef boost::shared_ptr<PlayerSession> ptr;

	PlayerSession(boost::asio::io_service& io_service, Game& game);

	boost::asio::ip::tcp::socket& getSocket();
	void start();
	void deliver(Packet::const_ptr packet);

	void handleReadHeader(boost::system::error_code const& error);
	void handleReadBody(boost::system::error_code const& error);
	void handleWrite(boost::system::error_code const& error);
};