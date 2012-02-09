#pragma once

#include <set>
#include <deque>

#include <PacketManager.h>
#include "Player.h"

class Game
{
private:
	const static int maxRecentPackets = 100;

	std::set<Player::ptr> players;
	std::deque<Packet::const_ptr> packetQueue;
	PacketManager& packetManager;

public:
	Game(PacketManager& packetManager);

	void join(Player::ptr player);
	void leave(Player::ptr player);
	void deliver(Packet::const_ptr packet);

	PacketManager& getPacketManager();
};