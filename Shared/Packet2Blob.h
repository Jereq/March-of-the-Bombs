#pragma once

#include "Packet.h"

class Packet2Blob
	: public Packet
{
private:
	const static uint16_t mId = 2;

	Packet2Blob(Packet2Blob& that);
	Packet2Blob& operator=(Packet2Blob& tmp);

protected:
	mutable char* blob;
	mutable uint16_t blobLength;

	virtual void pack() const;
	virtual void unpack() const;

public:
	Packet2Blob();		// Prototype constructor
	Packet2Blob(char const* data);
	Packet2Blob(char const* blob, uint16_t length);

	virtual ptr createPacket(char const* data, uint16_t length) const;
	virtual void dispatch(void* context) const;

	char const* getBlob() const;
	uint16_t getBlobLength() const;
};