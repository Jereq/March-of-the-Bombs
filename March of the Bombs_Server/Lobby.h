#pragma once

#include "Context.h"

#include <set>
#include <map>

class Lobby
	: public Context
{
private:
	boost::shared_ptr<PacketManager> packetManager;
	std::set<Player::ptr> newPlayers;

	typedef std::map<unsigned short, Context::ptr> context_map;
	context_map openGames;
	std::set<Context::ptr> runningGames;

	Packet12OpenGames::ptr openGamesPacket;

	unsigned short nextPlayerID;
	unsigned short nextGameID;

	const static size_t MAX_OPEN_GAMES = USHRT_MAX + 1;

public:
	Lobby(boost::shared_ptr<PacketManager> const& packetManager);

	virtual void join(Player::ptr const& player);
	virtual void leave(Player::ptr const& player);
	virtual void deliver(Packet::const_ptr const& packet);

	virtual boost::shared_ptr<PacketManager> getPacketManager();
	virtual std::set<Player::ptr> const& getPlayers() const;

	virtual void createGame(Player::ptr const& player, std::string const& mapName, unsigned short winLimit);
	virtual void removeGame(Context::ptr const& game);

	virtual void handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket6CreateGame(Packet6CreateGame::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket7JoinGame(Packet7JoinGame::const_ptr const& packet, Player::ptr const& sender);
	virtual void handlePacket11RequestOpenGames(Packet11RequestOpenGames::const_ptr const& packet, Player::ptr const& sender);
};