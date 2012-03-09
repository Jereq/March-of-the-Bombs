#include "Game.h"

#include <iostream>
#include <boost/foreach.hpp>

Game::Game(boost::shared_ptr<PacketManager> const& packetManager, std::string const& mapName)
	: packetManager(packetManager), mapName(mapName)
{
}

void Game::join(Player::ptr const& player)
{
	if (players.size() < gameSize)
	{
		std::cout << "[Game] " << player->getName() << " joined game" << std::endl;

		player->changeContext(shared_from_this());
		players.insert(player);

		if (players.size() == gameSize)
		{
			std::cout << "[Game] Game filled, starting..." << std::endl;

			std::vector<Player::ptr> playVec(players.begin(), players.end());

			Packet::ptr packet(new Packet8SetupGame(playVec[0]->getID(), glm::vec3(1, 0, 0), mapName, 0));
			playVec[1]->deliver(packet);

			packet.reset(new Packet8SetupGame(playVec[1]->getID(), glm::vec3(0, 1, 0), mapName, 1));
			playVec[0]->deliver(packet);
		}
	}
}

void Game::leave(Player::ptr const& player)
{
	std::cout << "[Game] " << player->getName() << " left the game" << std::endl;

	// TODO: Send good bye message
	std::cout << "[Game] Disconnecting last remaining player" << std::endl;
	players.clear();
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

std::string const& Game::getMapName() const
{
	return mapName;
}

void Game::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
}

void Game::handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
}

void Game::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet, Player::ptr const& sender)
{
	BOOST_FOREACH(Player::ptr const& player, players)
	{
		if (player != sender)
		{
			player->deliver(packet);
		}
	}
}