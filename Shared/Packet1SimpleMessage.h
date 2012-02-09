#pragma once

#include <string>

#include "Packet.h"

class Packet1SimpleMessage
	: public Packet
{
private:
	const static uint16_t mId = 1;

	Packet1SimpleMessage(Packet1SimpleMessage& tmp);
	Packet1SimpleMessage& operator=(Packet1SimpleMessage& tmp);

protected:
	mutable std::string msg;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet1SimpleMessage();
	Packet1SimpleMessage(char const* data, uint16_t length);
	Packet1SimpleMessage(std::string const& msg);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	std::string const& getMessage() const;
};