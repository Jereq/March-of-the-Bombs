#include "Acceptor.h"

#include <boost/bind.hpp>

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

Acceptor::Acceptor(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint, Lobby::ptr const& lobby)
	: io_service(io_service), acceptor(io_service, endpoint), lobby(lobby)
{
	startAccept();
}

void Acceptor::startAccept()
{
	Player::ptr newSession(new Player(io_service));
	acceptor.async_accept(newSession->getSocket(),
		boost::bind(&Acceptor::handleAccept, this, newSession,
			boost::asio::placeholders::error));
}

void Acceptor::handleAccept(Player::ptr player,	boost::system::error_code const& error)
{
	if (!error)
	{
		player->start(lobby);
		lobby->join(player);
	}

	startAccept();
}