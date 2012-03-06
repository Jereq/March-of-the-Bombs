#include "Lobby.h"

#include <boost/foreach.hpp>

Lobby::Lobby(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager)
{
}

void Lobby::join(Player::ptr& player)
{
	std::cout << "Player joined lobby" << std::endl;

	newPlayers.insert(player);
}

void Lobby::leave(Player::ptr& player)
{
	std::cout << "Player left lobby" << std::endl;

	newPlayers.erase(player);
}

void Lobby::deliver(Packet::const_ptr& packet)
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

void Lobby::addNewPlayer(Player::ptr const& newPlayer)
{
	newPlayer->start(shared_from_this());
	newPlayers.insert(newPlayer);
}