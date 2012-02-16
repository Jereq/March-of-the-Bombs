#include "Game.h"

#include <boost/foreach.hpp>

Game::Game(PacketManager& packetManager)
	: packetManager(packetManager)
{
}

void Game::join(Player::ptr player)
{
	std::cout << "Player joined" << std::endl;

	players.insert(player);

	//// Deliver history
	//BOOST_FOREACH(Packet::const_ptr packet, packetQueue)
	//{
	//	player->deliver(packet);
	//}
}

void Game::leave(Player::ptr player)
{
	std::cout << "Player left" << std::endl;

	players.erase(player);
}

void Game::deliver(Packet::const_ptr packet)
{
	packetQueue.push_back(packet);
	while (packetQueue.size() > maxRecentPackets)
	{
		packetQueue.pop_front();
	}

	BOOST_FOREACH(Player::ptr const& player, players)
	{
		player->deliver(packet);
	}
}

PacketManager& Game::getPacketManager()
{
	return packetManager;
}