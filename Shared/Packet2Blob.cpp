#include "Packet2Blob.h"

#include <WinSock2.h>

#include <iterator>
#include <algorithm>
using std::copy;

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
	uint16_t const* lengthP = reinterpret_cast<uint16_t const*>(&data[OFFSET_LENGTH]);
	uint16_t totalLength = ntohs(*lengthP);

	if (totalLength < MIN_SIZE)
	{
		return;
	}

	uint16_t const* idP = reinterpret_cast<uint16_t const*>(&data[OFFSET_ID]);
	
	if(id != ntohs(*idP))
	{
		return;
	}

	packedData = new char[totalLength];
	copy(data, data + totalLength, stdext::checked_array_iterator<char*>(packedData, totalLength));

	dataLength = totalLength;

	packed = true;
}

Packet2Blob::Packet2Blob(char const* blob, uint16_t length)
	: Packet(mId), blob(NULL), blobLength(length)
{
	if (length > Packet::MAX_LOAD_SIZE)
	{
		throw InvalidSizeException("Blob to big", shared_from_this());
	}

	uint16_t totalLength = blobLength + Packet::MIN_SIZE;
	packedData = new char[totalLength];
	dataLength = totalLength;
	
	this->blob = packedData + OFFSET_DATA;

	packHeader();

	copy(blob, blob + blobLength, stdext::make_checked_array_iterator<char*>(this->blob, blobLength));

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