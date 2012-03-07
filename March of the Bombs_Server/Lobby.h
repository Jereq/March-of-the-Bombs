#pragma once

#include "Context.h"

#include <set>

#include "Player.h"

class Lobby
	: public Context
{
private:
	boost::shared_ptr<PacketManager> packetManager;
	std::set<Player::ptr> newPlayers;
	std::set<Context::ptr> openGames;
	std::set<Context::ptr> runningGames;

public:
	Lobby(boost::shared_ptr<PacketManager> const& packetManager);

	virtual void join(Player::ptr const& player);
	virtual void leave(Player::ptr const& player);
	virtual void deliver(Packet::const_ptr const& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;

	virtual void createGame(Player::ptr const& player);

	virtual void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket2Blob(Packet2Blob::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender);
};