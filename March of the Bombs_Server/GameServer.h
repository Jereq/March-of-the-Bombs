#pragma once

#include <boost/asio.hpp>

#include "Game.h"
#include "PlayerSession.h"

class GameServer
{
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	PacketManager packetManager;

	Game game;

	void registerPackets();

public:
	typedef boost::shared_ptr<GameServer> ptr;
	typedef std::list<ptr> list;

	GameServer(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint);

	void startAccept();
	void handleAccept(PlayerSession::ptr session, boost::system::error_code const& error);
};