#include "Game.h"

#include <iostream>
#include <boost/foreach.hpp>

#include "Lobby.h"

Game::Game(Context::ptr const& parentLobby, unsigned short gameID, std::string const& mapName)
	: parentLobby(parentLobby), packetManager(parentLobby->getPacketManager()), mapName(mapName), gameID(gameID)
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

			Packet::ptr packet(new Packet8SetupGame(
				playVec[0]->getID(),
				playVec[0]->getName(),
				glm::vec3(1, 0, 0),
				mapName,
				0));
			playVec[1]->deliver(packet);

			packet.reset(new Packet8SetupGame(
				playVec[1]->getID(),
				playVec[1]->getName(),
				glm::vec3(0, 1, 0),
				mapName,
				1));
			playVec[0]->deliver(packet);
		}
	}
}

void Game::leave(Player::ptr const& player)
{
	std::cout << "[Game] " << player->getName() << " left the game" << std::endl;
	players.erase(player);

	// TODO: Send good bye message
	std::cout << "[Game] Disconnecting last remaining player" << std::endl;
	BOOST_FOREACH(Player::ptr const& lastPlayer, players)
	{
		lastPlayer->getSocket().close();
	}
	players.clear();

	Context::ptr parLock(parentLobby);
	Lobby* lobby = dynamic_cast<Lobby*>(parLock.get());
	if (lobby)
	{
		lobby->removeGame(shared_from_this());
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

std::string const& Game::getMapName() const
{
	return mapName;
}

unsigned short Game::getGameID() const
{
	return gameID;
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

void Game::handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
}

void Game::handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
}

void Game::handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet, Player::ptr const& sender)
{
	Packet15UpdatePlayerScore const* packet15 = reinterpret_cast<Packet15UpdatePlayerScore const*>(packet.get());

	sender->setScore(packet15->getNewScore());

	BOOST_FOREACH(Player::ptr const& player, players)
	{
		if (player != sender)
		{
			player->deliver(packet);
		}
	}
	
	const static float SCORE_TO_WIN = 1000.f;
	if (packet15->getNewScore() >= SCORE_TO_WIN)
	{
		if (players.size() == 2)
		{
			Player::ptr tPlayers[2];

			size_t i = 0;
			BOOST_FOREACH(Player::ptr const& player, players)
			{
				tPlayers[i++] = player;
			}

			Packet::ptr packet(new Packet16GameOver(
				tPlayers[0]->getID(), tPlayers[1]->getID(),
				tPlayers[0]->getScore(), tPlayers[1]->getScore()));

			tPlayers[0]->deliver(packet);
			tPlayers[1]->deliver(packet);

			std::cout << "[Game] " << sender->getName() << " won a game" << std::endl;
		}
	}
}