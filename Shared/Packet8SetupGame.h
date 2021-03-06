#pragma once

#include "Packet.h"

#include <glm/glm.hpp>

class Packet8SetupGame :
	public Packet
{
private:
	const static uint16_t mId = 8;
	const static size_t MAX_MAP_NAME_LENGTH = 256;

	Packet8SetupGame(Packet8SetupGame& that);
	Packet8SetupGame& operator=(Packet8SetupGame& tmp);

protected:
	mutable uint16_t opponentID;
	mutable std::string opponentName;
	mutable glm::vec3 opponentColor;
	mutable std::string mapName;
	mutable uint16_t baseID;
	mutable uint16_t winLimit;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet8SetupGame();		// Prototype constructor
	Packet8SetupGame(char const* data, uint16_t length);
	Packet8SetupGame(unsigned short opponentID, std::string const& opponentName,
		glm::vec3 const& opponentColor, std::string const& mapName,
		unsigned short baseID, unsigned short winLimit);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned short getOpponentID() const;
	std::string const& getOpponentName() const;
	glm::vec3 const& getOpponentColor() const;
	std::string const& getMapName() const;
	unsigned short getBaseID() const;
	unsigned short getWinLimit() const;
};