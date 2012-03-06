#pragma once

#include <set>
#include <boost/enable_shared_from_this.hpp>

#include <PacketManager.h>
#include "Player.h"

class Context
	: public boost::enable_shared_from_this<Context>
{
public:
	typedef boost::shared_ptr<Context> ptr;
	typedef boost::weak_ptr<Context> w_ptr;

	virtual void join(Player::ptr& player) = 0;
	virtual void leave(Player::ptr& player) = 0;
	virtual void deliver(Packet::const_ptr& packet) = 0;

	virtual boost::shared_ptr<PacketManager> getPacketManager() = 0;
	virtual std::set<Player::ptr> const& getPlayers() const = 0;
};