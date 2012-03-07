#pragma once

#include <set>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>
#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

#include "Player.h"

class Context
	: public boost::enable_shared_from_this<Context>
{
public:
	typedef boost::shared_ptr<Context> ptr;
	typedef boost::weak_ptr<Context> w_ptr;

	virtual void join(Player::ptr const& player) = 0;
	virtual void leave(Player::ptr const& player) = 0;
	virtual void deliver(Packet::const_ptr const& packet) = 0;

	virtual boost::shared_ptr<PacketManager> getPacketManager() = 0;
	virtual std::set<Player::ptr> const& getPlayers() const = 0;
	
	virtual void handlePacket1SimpleMessage(Packet1SimpleMessage::const_ptr const& packet, Player::ptr const& sender) = 0;
	virtual void handlePacket2Blob(Packet2Blob::const_ptr const& packet, Player::ptr const& sender) = 0;
	virtual void handlePacket3Login(Packet3Login::const_ptr const& packet, Player::ptr const& sender) = 0;
	virtual void handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet, Player::ptr const& sender) = 0;
};