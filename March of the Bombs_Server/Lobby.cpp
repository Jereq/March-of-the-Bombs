#include "Lobby.h"

#include <boost/foreach.hpp>

#include "Game.h"

Lobby::Lobby(boost::shared_ptr<PacketManager> const& packetManager)
	: packetManager(packetManager)
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
	newPlayers.erase(player);
	openGames.insert(Context::ptr(new Game(packetManager, player)));
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
	sender->setID(1);

	std::cout << "[Lobby] " << sender->getName() << " logged in with ID: " << sender->getID() << std::endl;

	sender->deliver(Packet::ptr(new Packet4LoginAccepted(1)));
}

void Lobby::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender)
{

}