#pragma once
#include <boost\shared_ptr.hpp>

class Block
{
public:
	Block(void);
	~Block(void);
	typedef boost::shared_ptr<Block> ptr;
};

