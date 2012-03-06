#include "Player.h"

#include <boost/bind.hpp>
using boost::asio::ip::tcp;

#include "Context.h"

Player::Player(boost::asio::io_service& io_service)
	: socket(io_service)
{
}

tcp::socket& Player::getSocket()
{
	return socket;
}

void Player::start(Context::ptr const& context)
{
	this->context = context;
	packetManager = context->getPacketManager();

	boost::asio::async_read(socket,
		boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
		boost::bind(
		&Player::handleReadHeader, shared_from_this(),
		boost::asio::placeholders::error));
}

void Player::deliver(Packet::const_ptr packet)
{
	bool writeInProgress = !writePackets.empty();
	writePackets.push_back(packet);
	if (!writeInProgress)
	{
		boost::asio::async_write(socket,
			boost::asio::buffer(writePackets.front()->getData(),
				writePackets.front()->getDataLength()),
			boost::bind(&Player::handleWrite, shared_from_this(),
				boost::asio::placeholders::error));
	}
}

void Player::handleReadHeader(boost::system::error_code const& error)
{
	if (!error)
	{
		packetLength = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_LENGTH]));
		packetId = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_ID]));
		boost::asio::async_read(socket,
			boost::asio::buffer(&readBuffer[Packet::MIN_SIZE], packetLength - Packet::MIN_SIZE),
			boost::bind(&Player::handleReadBody, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		context->leave(shared_from_this());
	}
}

void Player::handleReadBody(boost::system::error_code const& error)
{
	if (!error)
	{
		Packet::ptr packet = packetManager->createPacket(readBuffer, packetLength, packetId);

		if (packet)
		{
			packet->dispatch(&context);
		}

		boost::asio::async_read(socket,
			boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
			boost::bind(&Player::handleReadHeader, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		context->leave(shared_from_this());
	}
}

void Player::handleWrite(boost::system::error_code const& error)
{
	if (!error)
	{
		writePackets.pop_front();
		if (!writePackets.empty())
		{
			boost::asio::async_write(socket,
				boost::asio::buffer(writePackets.front()->getData(),
					writePackets.front()->getDataLength()),
				boost::bind(&Player::handleWrite, shared_from_this(),
					boost::asio::placeholders::error));
		}
	}
	else
	{
		context->leave(shared_from_this());
	}
}