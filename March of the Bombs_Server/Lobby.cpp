#include "Lobby.h"

#include <boost/foreach.hpp>

#include "Game.h"

Lobby::Lobby(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager), nextPlayerID(1), nextGameID(0)
{
}

void Lobby::join(Player::ptr const& player)
{
	newPlayers.insert(player);
}

void Lobby::leave(Player::ptr const& player)
{
	std::cout << "[Lobby] " << player->getName() << " left the lobby" << std::endl;

	newPlayers.erase(player);
}

void Lobby::deliver(Packet::const_ptr const& packet)
{
	BOOST_FOREACH(Player::ptr const& player, newPlayers)
	{
		player->deliver(packet);
	}
}

boost::shared_ptr<PacketManager> Lobby::getPacketManager()
{
	return packetManager;
}

std::set<Player::ptr> const& Lobby::getPlayers() const
{
	return newPlayers;
}

void Lobby::createGame(Player::ptr const& player, std::string const& mapName)
{
	if (openGames.size() < MAX_OPEN_GAMES)
	{
		newPlayers.erase(player);

		Context::ptr newGame(new Game(packetManager, mapName));
		newGame->join(player);

		while (openGames.count(nextGameID) == 1)
		{
			nextGameID++;
		}

		openGames[nextGameID++] = newGame;

		std::cout << "[Lobby] Game created with ID: " << nextGameID - 1 << std::endl;
	}
}

void Lobby::handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender)
{
	Packet3Login const* packet3 = static_cast<Packet3Login const*>(packet.get());

	sender->setName(packet3->getName());
	sender->setID(nextPlayerID++);

	std::cout << "[Lobby] " << sender->getName() << " logged in with ID: " << sender->getID() << std::endl;

	sender->deliver(Packet::ptr(new Packet4LoginAccepted(sender->getID())));
}

void Lobby::handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet, Player::ptr const& sender)
{
	Packet6CreateGame const* packet6 = static_cast<Packet6CreateGame const*>(packet.get());

	createGame(sender, packet6->getMapName());
}

void Lobby::handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet, Player::ptr const& sender)
{
	Packet7JoinGame const* packet7 = static_cast<Packet7JoinGame const*>(packet.get());

	unsigned short gameID = packet7->getGameID();

	if (openGames.count(gameID) == 1)
	{
		Game::ptr game = openGames[gameID];
		game->join(sender);

		newPlayers.erase(sender);

		openGames.erase(gameID);
		runningGames.insert(game);
	}
}