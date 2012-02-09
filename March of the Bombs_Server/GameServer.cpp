#include "GameServer.h"

#include <boost/bind.hpp>

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

void GameServer::registerPackets()
{
	packetManager.addPacketPrototype(Packet::ptr(new Packet1SimpleMessage()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet2Blob()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet3Login()));
}

GameServer::GameServer(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint const& endpoint)
	: io_service(io_service), acceptor(io_service, endpoint), game(packetManager)
{
	registerPackets();
	startAccept();
}

void GameServer::startAccept()
{
	PlayerSession::ptr newSession(new PlayerSession(io_service, game));
	acceptor.async_accept(newSession->getSocket(),
		boost::bind(&GameServer::handleAccept, this, newSession,
			boost::asio::placeholders::error));
}

void GameServer::handleAccept(PlayerSession::ptr session,
	boost::system::error_code const& error)
{
	if (!error)
	{
		session->start();
	}

	startAccept();
}