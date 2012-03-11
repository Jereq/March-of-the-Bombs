#pragma once

#include "Packet.h"

#include <vector>
#include <glm/glm.hpp>

class Packet14RemoveBlocks :
	public Packet
{
private:
	const static uint16_t mId = 14;

	Packet14RemoveBlocks(Packet14RemoveBlocks& that);
	Packet14RemoveBlocks& operator=(Packet14RemoveBlocks& tmp);

protected:
	mutable std::vector<glm::ivec2> blocks;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet14RemoveBlocks();		// Prototype constructor
	Packet14RemoveBlocks(char const* data, uint16_t length);
	Packet14RemoveBlocks(std::vector<glm::ivec2> const& blocks);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	std::vector<glm::ivec2> const& getBlocks() const;
};