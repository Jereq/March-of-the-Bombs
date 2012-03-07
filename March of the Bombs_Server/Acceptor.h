#pragma once

#include <boost/asio.hpp>

#include "Game.h"
#include "Player.h"
#include "Lobby.h"

class Acceptor
{
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::shared_ptr<PacketManager> packetManager;

	Lobby::ptr lobby;

public:
	typedef boost::shared_ptr<Acceptor> ptr;
	typedef std::list<ptr> list;

	Acceptor(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint, Lobby::ptr const& lobby);

	void startAccept();
	void handleAccept(Player::ptr player, boost::system::error_code const& error);
};