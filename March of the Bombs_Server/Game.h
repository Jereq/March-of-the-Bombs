#pragma once

#include "Context.h"

class Game
	: public Context
{
private:
	const static int gameSize = 2;

	boost::shared_ptr<PacketManager> packetManager;
	std::set<Player::ptr> players;

	Context::w_ptr parentLobby;
	unsigned short gameID;

	bool gameOver;
	unsigned short winLimit;

	std::string mapName;

public:
	Game(Context::ptr const& parentLobby, unsigned short gameID,
		std::string const& mapName, unsigned short winLimit);

	virtual void join(Player::ptr const& player);
	virtual void leave(Player::ptr const& player);
	virtual void deliver(Packet::const_ptr const& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;
	virtual std::string const& getMapName() const;
	virtual unsigned short getGameID() const;

	virtual void handlePacket5EntityMove(Packet5EntityMove::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket9SpawnBomb(Packet9SpawnBomb::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket13RemoveBomb(Packet13RemoveBomb::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket14RemoveBlocks(Packet14RemoveBlocks::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket15UpdatePlayerScore(Packet15UpdatePlayerScore::const_ptr const& packet, Player::ptr const& sender);
};