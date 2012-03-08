#include "Game.h"

#include <iostream>
#include <boost/foreach.hpp>

Game::Game(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager)
{
}

void Game::join(Player::ptr const& player)
{
	if (players.size() < gameSize)
	{
		std::cout << player->getName() << " joined game" << std::endl;

		player->changeContext(shared_from_this());
		players.insert(player);

		if (players.size() == gameSize)
		{
			std::cout << "Game filled, starting..." << std::endl;
		}
	}
}

void Game::leave(Player::ptr const& player)
{
	std::cout << "Player left" << std::endl;

	players.erase(player);

	if (players.size() == 1)
	{
		// TODO: Send good bye message
		players.clear();
	}
}

void Game::deliver(Packet::const_ptr const& packet)
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

void Game::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);

	Packet5EntityMove const* packet5 = static_cast<Packet5EntityMove const*>(packet.get());
}