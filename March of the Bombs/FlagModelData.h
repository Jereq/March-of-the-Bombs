#pragma once

#include "Model3DS.h"

#include <boost/shared_ptr.hpp>

class FlagModelData :
	public Model3DS
{
public:
	typedef boost::shared_ptr<FlagModelData> ptr;

private:
	static ptr instance;

protected:
	FlagModelData();

public:
	static ptr getInstance();
};