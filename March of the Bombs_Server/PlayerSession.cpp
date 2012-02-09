#include "PlayerSession.h"

#include <boost/bind.hpp>
using boost::asio::ip::tcp;

PlayerSession::PlayerSession(boost::asio::io_service& io_service, Game& game)
	: socket(io_service), game(game), packetManager(game.getPacketManager())
{
}

tcp::socket& PlayerSession::getSocket()
{
	return socket;
}

void PlayerSession::start()
{
	game.join(shared_from_this());
	boost::asio::async_read(socket,
		boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
		boost::bind(
		&PlayerSession::handleReadHeader, shared_from_this(),
		boost::asio::placeholders::error));
}

void PlayerSession::deliver(Packet::const_ptr packet)
{
	bool writeInProgress = !writePackets.empty();
	writePackets.push_back(packet);
	if (!writeInProgress)
	{
		boost::asio::async_write(socket,
			boost::asio::buffer(writePackets.front()->getData(),
				writePackets.front()->getDataLength()),
			boost::bind(&PlayerSession::handleWrite, shared_from_this(),
				boost::asio::placeholders::error));
	}
}

void PlayerSession::handleReadHeader(boost::system::error_code const& error)
{
	if (!error)
	{
		packetLength = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_LENGTH]));
		packetId = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_ID]));
		boost::asio::async_read(socket,
			boost::asio::buffer(&readBuffer[Packet::MIN_SIZE], packetLength - Packet::MIN_SIZE),
			boost::bind(&PlayerSession::handleReadBody, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		game.leave(shared_from_this());
	}
}

void PlayerSession::handleReadBody(boost::system::error_code const& error)
{
	if (!error)
	{
		Packet::ptr packet = packetManager.createPacket(readBuffer, packetLength, packetId);

		if (packet)
		{
			packet->dispatch(&game);
		}

		boost::asio::async_read(socket,
			boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
			boost::bind(&PlayerSession::handleReadHeader, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		game.leave(shared_from_this());
	}
}

void PlayerSession::handleWrite(boost::system::error_code const& error)
{
	if (!error)
	{
		writePackets.pop_front();
		if (!writePackets.empty())
		{
			boost::asio::async_write(socket,
				boost::asio::buffer(writePackets.front()->getData(),
					writePackets.front()->getDataLength()),
				boost::bind(&PlayerSession::handleWrite, shared_from_this(),
					boost::asio::placeholders::error));
		}
	}
	else
	{
		game.leave(shared_from_this());
	}
}