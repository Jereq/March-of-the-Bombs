#pragma once

#include "Model3DS.h"

#include <boost/shared_ptr.hpp>

class HQModelData : 
	public Model3DS
{
public:
	typedef boost::shared_ptr<HQModelData> ptr;

private:
	static ptr instance;

protected:
	HQModelData();

public:
	static ptr getInstance();
};

