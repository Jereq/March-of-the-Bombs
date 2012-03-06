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

public:
	Lobby(boost::shared_ptr<PacketManager> const& packetManager);

	virtual void join(Player::ptr& player);
	virtual void leave(Player::ptr& player);
	virtual void deliver(Packet::const_ptr& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;

	virtual void addNewPlayer(Player::ptr const& newPlayer);
};