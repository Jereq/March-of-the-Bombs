#pragma once

#include <boost/asio.hpp>

#include "Game.h"
#include "Player.h"
#include "Lobby.h"

class GameServer
{
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::shared_ptr<PacketManager> packetManager;

	Game::ptr game;
	Lobby& lobby;

	void registerPackets();

public:
	typedef boost::shared_ptr<GameServer> ptr;
	typedef std::list<ptr> list;

	GameServer(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint, Lobby& lobby);

	void startAccept();
	void handleAccept(Player::ptr session, boost::system::error_code const& error);
};