#pragma once

#include <string>

#include "packet.h"

class Packet3Login :
	public Packet
{
private:
	const static uint16_t mId = 3;
	const static int MAX_NAME_LENGHT = 16;

	Packet3Login(Packet3Login& that);
	Packet3Login& operator=(Packet3Login& tmp);

protected:
	mutable std::string name;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet3Login();		// Prototype constructor
	Packet3Login(char const* data, uint16_t length);
	Packet3Login(std::string const& name);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	std::string const& getName() const;
};