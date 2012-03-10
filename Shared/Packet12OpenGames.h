#pragma once

#include "Packet.h"

#include <vector>

#include "OpenGame.h"

class Packet12OpenGames :
	public Packet
{
private:
	const static uint16_t mId = 12;

	Packet12OpenGames(Packet12OpenGames& that);
	Packet12OpenGames& operator=(Packet12OpenGames& tmp);

protected:
	mutable std::vector<OpenGame> openGames;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet12OpenGames();		// Prototype constructor
	Packet12OpenGames(char const* data, uint16_t length);
	Packet12OpenGames(std::vector<OpenGame> const& openGames);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	std::vector<OpenGame> const& getOpenGames() const;
};