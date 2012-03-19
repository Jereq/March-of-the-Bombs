#pragma once

#include <deque>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>
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
#include <Packet16GameOver.h>

class Context;

class Player
	: public boost::enable_shared_from_this<Player>
{
private:
	boost::asio::ip::tcp::socket socket;
	boost::weak_ptr<Context> context;
	boost::shared_ptr<PacketManager> packetManager;

	char readBuffer[Packet::MAX_SIZE];
	uint16_t packetLength;
	uint16_t packetId;
	std::deque<Packet::const_ptr> writePackets;

	std::string name;
	unsigned int ID;
	float score;

public:
	typedef boost::shared_ptr<Player> ptr;

	Player(boost::asio::io_service& io_service);

	boost::asio::ip::tcp::socket& getSocket();
	void start(boost::weak_ptr<Context> const& context);
	void deliver(Packet::const_ptr packet);

	void handleReadHeader(boost::system::error_code const& error);
	void handleReadBody(boost::system::error_code const& error);
	void handleWrite(boost::system::error_code const& error);

	boost::weak_ptr<Context> getContext() const;
	void changeContext(boost::weak_ptr<Context> const& constext);
	
	void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet);
	void handlePacket2Blob(Packet2Blob::const_ptr const& packet);
	void handlePacket3Login(Packet3Login::const_ptr const& packet);
	void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet);
	void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet);
	void handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet);
	void handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet);
	void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet);
	void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet);
	void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet);
	void handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet);
	void handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet);
	void handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet);
	void handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet);
	void handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet);
	void handlePacket16GameOver(Packet16GameOver::const_ptr const& packet);

	std::string const& getName() const;
	void setName(std::string const& newName);

	unsigned int getID() const;
	void setID(unsigned int newID);

	float getScore() const;
	void setScore(float newScore);
};