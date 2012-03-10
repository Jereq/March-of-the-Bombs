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

void Player::start(Context::w_ptr const& context)
{
	this->context = context;
	packetManager = context.lock()->getPacketManager();

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
		Context::ptr conLock(context);
		if (conLock)
		{
			conLock->leave(shared_from_this());
		}
	}
}

void Player::handleReadBody(boost::system::error_code const& error)
{
	if (!error)
	{
		Packet::ptr packet = packetManager->createPacket(readBuffer, packetLength, packetId);

		if (packet)
		{
			packet->dispatch(&shared_from_this());
		}

		boost::asio::async_read(socket,
			boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
			boost::bind(&Player::handleReadHeader, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		Context::ptr conLock(context);
		if (conLock)
		{
			conLock->leave(shared_from_this());
		}
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
		Context::ptr conLock(context);
		if (conLock)
		{
			conLock->leave(shared_from_this());
		}
	}
}

void Player::changeContext(Context::w_ptr const& context)
{
	this->context = context;
}

Context::w_ptr Player::getContext() const
{
	return context;
}

void Player::handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket1SimpleMessage(packet, shared_from_this());
	}
}

void Player::handlePacket2Blob(Packet2Blob::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket2Blob(packet, shared_from_this());
	}
}

void Player::handlePacket3Login(Packet3Login::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket3Login(packet, shared_from_this());
	}
}

void Player::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket4LoginAccepted(packet, shared_from_this());
	}
}

void Player::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket5EntityMove(packet, shared_from_this());
	}
}

void Player::handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket6CreateGame(packet, shared_from_this());
	}
}

void Player::handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket7JoinGame(packet, shared_from_this());
	}
}

void Player::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket8SetupGame(packet, shared_from_this());
	}
}

void Player::handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket9SpawnBomb(packet, shared_from_this());
	}
}

void Player::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket10PlayerReady(packet, shared_from_this());
	}
}

void Player::handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket11RequestOpenGames(packet, shared_from_this());
	}
}

void Player::handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet)
{
	Context::ptr con = context.lock();
	if (con)
	{
		con->handlePacket12OpenGames(packet, shared_from_this());
	}
}

std::string const& Player::getName() const
{
	return name;
}

void Player::setName(std::string const& newName)
{
	name = newName;
}

unsigned int Player::getID() const
{
	return ID;
}

void Player::setID(unsigned int newID)
{
	ID = newID;
}