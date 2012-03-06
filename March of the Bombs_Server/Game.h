#pragma once

#include "Context.h"

class Game
	: public Context
{
private:
	const static int gameSize = 2;

	boost::shared_ptr<PacketManager> packetManager;
	std::set<Player::ptr> players;

public:
	Game(boost::shared_ptr<PacketManager> const& packetManager);

	virtual void join(Player::ptr& player);
	virtual void leave(Player::ptr& player);
	virtual void deliver(Packet::const_ptr& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;
};