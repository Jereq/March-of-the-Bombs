#pragma once

#include "Context.h"

class Game
	: public Context
{
private:
	const static int gameSize = 2;

	boost::shared_ptr<PacketManager> packetManager;
	std::set<Player::ptr> players;

	std::string mapName;

public:
	Game(boost::shared_ptr<PacketManager> const& packetManager, std::string const& mapName);

	virtual void join(Player::ptr const& player);
	virtual void leave(Player::ptr const& player);
	virtual void deliver(Packet::const_ptr const& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;
	virtual std::string const& getMapName() const;

	virtual void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet, Player::ptr const& sender);
};