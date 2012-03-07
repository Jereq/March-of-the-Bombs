#include "Game.h"

#include <iostream>
#include <boost/foreach.hpp>

Game::Game(boost::shared_ptr<PacketManager> const& packetManager, Player::ptr const& creator)
	: packetManager(packetManager)
{
	join(creator);
}

void Game::join(Player::ptr const& player)
{
	if (players.size() < gameSize)
	{
		std::cout << "Player joined" << std::endl;

		players.insert(player);
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

void Game::handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);
	
	Packet1SimpleMessage const* packet1 = static_cast<Packet1SimpleMessage const*>(packet.get());
	std::cout << "<Client> [Game] " << packet1->getMessage() << std::endl;
}

void Game::handlePacket2Blob(Packet2Blob::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);

	Packet2Blob const* packet2 = static_cast<Packet2Blob const*>(packet.get());
	std::cout << "Received blob from client: " << packet2->getBlobLength() << " bytes" << std::endl;
}

void Game::handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender)
{
	deliver(packet);

	Packet3Login const* packet3 = static_cast<Packet3Login const*>(packet.get());
	std::cout << packet3->getName() << " logged in" << std::endl;
}

void Game::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender)
{

}