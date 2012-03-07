#include "Lobby.h"

#include <boost/foreach.hpp>

#include "Game.h"

Lobby::Lobby(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager), nextPlayerID(1), nextGameID(0)
{
}

void Lobby::join(Player::ptr const& player)
{
	std::cout << "Player joined lobby" << std::endl;

	newPlayers.insert(player);
}

void Lobby::leave(Player::ptr const& player)
{
	std::cout << "Player left lobby" << std::endl;

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

void Lobby::createGame(Player::ptr const& player)
{
	if (openGames.size() < MAX_OPEN_GAMES)
	{
		newPlayers.erase(player);

		Context::ptr newGame(new Game(packetManager));
		newGame->join(player);

		while (openGames.count(nextGameID) == 1)
		{
			nextGameID++;
		}

		openGames[nextGameID++] = newGame;

		std::cout << "Game " << nextGameID - 1 << " created" << std::endl;
	}
}

void Lobby::handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
	
	Packet1SimpleMessage const* packet1 = static_cast<Packet1SimpleMessage const*>(packet.get());
	std::cout << "[Lobby] <Client> " << packet1->getMessage() << std::endl;
}

void Lobby::handlePacket2Blob(Packet2Blob::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);

	Packet2Blob const* packet2 = static_cast<Packet2Blob const*>(packet.get());
	std::cout << "[Lobby] Received blob from client: " << packet2->getBlobLength() << " bytes" << std::endl;
}

void Lobby::handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender)
{
	Packet3Login const* packet3 = static_cast<Packet3Login const*>(packet.get());

	sender->setName(packet3->getName());
	sender->setID(nextPlayerID++);

	std::cout << "[Lobby] " << sender->getName() << " logged in with ID: " << sender->getID() << std::endl;

	sender->deliver(Packet::ptr(new Packet4LoginAccepted(sender->getID())));
}

void Lobby::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender)
{

}

void Lobby::handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender)
{
	std::cout << "[Lobby] Received Packet5EntityMove from a player" << std::endl;
}

void Lobby::handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet, Player::ptr const& sender)
{
	Packet6CreateGame const* packet6 = static_cast<Packet6CreateGame const*>(packet.get());

	createGame(sender);
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