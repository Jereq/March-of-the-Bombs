#pragma once
#pragma comment (lib, "Shared.lib")

#include <cstdint>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Packet
	: public boost::enable_shared_from_this<Packet>
{
private:
	Packet(Packet& tmp);
	Packet& operator=(Packet& tmp);

protected:
	const uint16_t id;
	uint16_t dataLength;

	mutable char* packedData;
	mutable bool packed;
	mutable bool unpacked;

	Packet(uint16_t id);
	Packet(uint16_t id, char const* data, uint16_t length);

	virtual void pack() const = 0;
	virtual void unpack() const = 0;

	void packHeader() const;
	bool checkHeader() const;

public:
	typedef boost::shared_ptr<Packet> ptr;
	typedef boost::shared_ptr<const Packet> const_ptr;

	const static int OFFSET_LENGTH = 0;
	const static int OFFSET_ID = OFFSET_LENGTH + sizeof(uint16_t);
	const static int OFFSET_DATA = OFFSET_ID + sizeof(uint16_t);

	const static int MIN_SIZE = sizeof(uint16_t) * 2;	// Size of header
	const static int MAX_SIZE = UINT16_MAX;				// Max value of length
	const static int MAX_LOAD_SIZE = MAX_SIZE - MIN_SIZE;	// Max size of data in order to not pass the MAX_SIZE limit

	virtual ~Packet();

	virtual ptr createPacket(char const* data, uint16_t length) const = 0;
	virtual void dispatch(void* context) const = 0;

	virtual char const* getData() const;
	virtual uint16_t getId() const;
	virtual uint16_t getDataLength() const;
};