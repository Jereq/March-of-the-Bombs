#include "Game.h"

#include <iostream>
#include <boost/foreach.hpp>

Game::Game(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager)
{
}

void Game::join(Player::ptr& player)
{
	if (players.size() < gameSize)
	{
		std::cout << "Player joined" << std::endl;

		players.insert(player);
	}
}

void Game::leave(Player::ptr& player)
{
	std::cout << "Player left" << std::endl;

	players.erase(player);

	if (players.size() == 1)
	{
		// TODO: Send good bye message
		players.clear();
	}
}

void Game::deliver(Packet::const_ptr& packet)
{
	BOOST_FOREACH(Player::ptr const& player, players)
	{
		player->deliver(packet);
	}
}

boost::shared_ptr<PacketManager> Game::getPacketManager()
{
	return packetManager;
}

std::set<Player::ptr> const& Game::getPlayers() const
{
	return players;
}