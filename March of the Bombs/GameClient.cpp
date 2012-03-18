#include "GameClient.h"

#include <boost/bind.hpp>

#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>
#include <Packet4LoginAccepted.h>
#include <Packet5EntityMove.h>
#include <Packet6CreateGame.h>
#include <Packet7JoinGame.h>
#include <Packet8SetupGame.h>
#include <Packet9SpawnBomb.h>
#include <Packet10PlayerReady.h>
#include <Packet11RequestOpenGames.h>
#include <Packet12OpenGames.h>
#include <Packet13RemoveBomb.h>
#include <Packet14RemoveBlocks.h>
#include <Packet15UpdatePlayerScore.h>

void GameClient::registerPackets()
{
	packetManager.addPacketPrototype(Packet::ptr(new Packet1SimpleMessage()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet2Blob()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet3Login()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet4LoginAccepted()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet5EntityMove()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet6CreateGame()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet7JoinGame()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet8SetupGame()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet9SpawnBomb()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet10PlayerReady()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet11RequestOpenGames()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet12OpenGames()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet13RemoveBomb()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet14RemoveBlocks()));
	packetManager.addPacketPrototype(Packet::ptr(new Packet15UpdatePlayerScore()));
}

void GameClient::handleConnect(boost::system::error_code const& error)
{
	if (!error)
	{
		startRead();

		connected = true;

		if (!writePackets.empty())
		{
			startWrite();
		}
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
		pushReceivedPacket(packet);

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
	if (!writeInProgress && connected)
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

void GameClient::pushReceivedPacket(Packet::ptr const& packet)
{
	boost::lock_guard<boost::mutex> lock(receivedPacketsMutex);

	receivedPackets.push_back(packet);
}

GameClient::GameClient(std::string const& host, std::string const& port)
	: socket(io_service), connected(false)
{
	registerPackets();

	using boost::asio::ip::tcp;

	tcp::resolver resolver(io_service);
	tcp::resolver::query query(host, port);
	tcp::resolver::iterator endpointIterator = resolver.resolve(query);

	boost::asio::async_connect(socket, endpointIterator,
		boost::bind(&GameClient::handleConnect, this,
			boost::asio::placeholders::error));

	start();
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
	boost::lock_guard<boost::mutex> lock(receivedPacketsMutex);

	Packet::ptr first = receivedPackets.front();
	receivedPackets.pop_front();

	return first;
}

bool GameClient::isRunning()
{
	return !thread.timed_join(boost::get_system_time());
}