#pragma once

#include "Packet.h"

#include <glm/glm.hpp>

class Packet5EntityMove :
	public Packet
{
private:
	const static uint16_t mId = 5;

	Packet5EntityMove(Packet5EntityMove& that);
	Packet5EntityMove& operator=(Packet5EntityMove& tmp);

protected:
	mutable uint16_t playerID;
	mutable uint16_t entityID;
	mutable glm::vec3 position;
	mutable glm::vec3 rotation;
	mutable glm::vec3 velocity;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet5EntityMove();		// Prototype constructor
	Packet5EntityMove(char const* data, uint16_t length);
	Packet5EntityMove(unsigned short playerID, unsigned short entityID,
		glm::vec3 const& position, glm::vec3 const& rotation, glm::vec3 const& velocity);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	unsigned short getPlayerID() const;
	unsigned short getEntityID() const;
	glm::vec3 const& getPosition() const;
	glm::vec3 const& getRotation() const;
	glm::vec3 const& getVelocity() const;
};