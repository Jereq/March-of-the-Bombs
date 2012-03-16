#pragma once

#include <set>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>

#include "Player.h"

class Context
	: public boost::enable_shared_from_this<Context>
{
public:
	typedef boost::shared_ptr<Context> ptr;
	typedef boost::weak_ptr<Context> w_ptr;

	virtual void join(Player::ptr const& player) = 0;
	virtual void leave(Player::ptr const& player) = 0;
	virtual void deliver(Packet::const_ptr const& packet) = 0;

	virtual boost::shared_ptr<PacketManager> getPacketManager() = 0;
	virtual std::set<Player::ptr> const& getPlayers() const = 0;
	
	virtual void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket2Blob(Packet2Blob::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet, Player::ptr const& sender) {};
	virtual void handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet, Player::ptr const& sender) {};
};