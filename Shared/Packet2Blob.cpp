#include "Packet2Blob.h"

#include "Pack.h"
#include "InvalidSizeException.h"

void Packet2Blob::pack() const
{
	return;	// If it has data, then it is already packed.
}

void Packet2Blob::unpack() const
{
	if (!packedData)
	{
		return;
	}

	if (!checkHeader())
	{
		return;
	}

	blob = packedData + OFFSET_DATA;
	blobLength = dataLength - OFFSET_DATA;

	unpacked = true;
}

Packet2Blob::Packet2Blob()
	: Packet(mId), blob(NULL), blobLength(0)
{
}

Packet2Blob::Packet2Blob(char const* data)
	: Packet(mId), blob(NULL), blobLength(0)
{
	uint16_t totalLength;
	util::unpack(&totalLength, 1, &data[OFFSET_LENGTH]);

	if (totalLength < MIN_SIZE)
	{
		return;
	}

	uint16_t tId;
	util::unpack(&tId, 1, &data[OFFSET_ID]);
	
	if(id != tId)
	{
		return;
	}

	dataLength = totalLength;
	packedData = new char[dataLength];
	util::unpack(packedData, dataLength, data);

	packed = true;
}

Packet2Blob::Packet2Blob(char const* blob, uint16_t length)
	: Packet(mId), blob(NULL), blobLength(length)
{
	if (length > Packet::MAX_LOAD_SIZE)
	{
		throw InvalidSizeException("Blob to big", Packet::ptr());
	}

	dataLength = blobLength + Packet::MIN_SIZE;
	packedData = new char[dataLength];
	
	this->blob = packedData + OFFSET_DATA;

	packHeader();
	util::pack(blob, dataLength, this->blob);

	packed = true;
	unpacked = true;
}

Packet::ptr Packet2Blob::createPacket(char const* data, uint16_t length) const
{
	return Packet::ptr(new Packet2Blob(data));
}

char const* Packet2Blob::getBlob() const
{
	if (!unpacked)
	{
		unpack();
	}

	return blob;
}

uint16_t Packet2Blob::getBlobLength() const
{
	if (!unpacked)
	{
		unpack();
	}

	return blobLength;
}