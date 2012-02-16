#include "GameClient.h"

#include <boost/bind.hpp>

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

void GameClient::registerPackets()
{
	packetManager.addPacketPrototype(Packet::ptr(new Packet1SimpleMessage()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet2Blob()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet3Login()));
}

void GameClient::handleConnect(boost::system::error_code const& error)
{
	if (!error)
	{
		startRead();
	}
}

void GameClient::handleReadHeader(boost::system::error_code const& error)
{
	if (!error)
	{
		packetLength = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_LENGTH]));
		packetId = ntohs(*reinterpret_cast<uint16_t*>(&readBuffer[Packet::OFFSET_ID]));

		boost::asio::async_read(socket,
			boost::asio::buffer(&readBuffer[Packet::MIN_SIZE], packetLength - Packet::MIN_SIZE),
			boost::bind(&GameClient::handleReadBody, this,
				boost::asio::placeholders::error));
	}
	else
	{
		doClose();
	}
}

void GameClient::handleReadBody(boost::system::error_code const& error)
{
	if (!error)
	{
		Packet::ptr packet = packetManager.createPacket(readBuffer, packetLength, packetId);
		receivedPackets.push_back(packet);

		startRead();
	}
	else
	{
		doClose();
	}
}

void GameClient::handleWrite(boost::system::error_code const& error)
{
	if (!error)
	{
		writePackets.pop_front();
		if (!writePackets.empty())
		{
			startWrite();
		}
	}
	else
	{
		doClose();
	}
}

void GameClient::doWrite(Packet::ptr packet)
{
	bool writeInProgress = !writePackets.empty();
	writePackets.push_back(packet);
	if (!writeInProgress)
	{
		startWrite();
	}
}

void GameClient::doClose()
{
	socket.close();
}

void GameClient::startWrite()
{
	boost::asio::async_write(socket,
		boost::asio::buffer(writePackets.front()->getData(), writePackets.front()->getDataLength()),
		boost::bind(&GameClient::handleWrite, this,
			boost::asio::placeholders::error));
}

void GameClient::startRead()
{
	boost::asio::async_read(socket,
		boost::asio::buffer(readBuffer, Packet::MIN_SIZE),
		boost::bind(&GameClient::handleReadHeader, this,
		boost::asio::placeholders::error));
}

GameClient::GameClient(boost::asio::io_service& io_service,
	boost::asio::ip::tcp::resolver::iterator endpointIterator)
	: io_service(io_service), socket(io_service)
{
	registerPackets();

	boost::asio::async_connect(socket, endpointIterator,
		boost::bind(&GameClient::handleConnect, this,
			boost::asio::placeholders::error));
}

GameClient::~GameClient()
{
	close();
	thread.join();
}

void GameClient::start()
{
	thread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
}

void GameClient::write(Packet::ptr packet)
{
	io_service.post(boost::bind(&GameClient::doWrite, this, packet));
}

void GameClient::close()
{
	io_service.post(boost::bind(&GameClient::doClose, this));
}

bool GameClient::hasReceivedPackets() const
{
	return !receivedPackets.empty();
}

Packet::ptr GameClient::popReceivedPacket()
{
	Packet::ptr first = receivedPackets.front();
	receivedPackets.pop_front();

	return first;
}

bool GameClient::isRunning()
{
	return !thread.timed_join(boost::get_system_time());
}