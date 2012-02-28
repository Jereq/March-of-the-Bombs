#pragma once

#include "Model3DS.h"

#include <boost/shared_ptr.hpp>

class StandardBombModelData :
	public Model3DS
{
public:
	typedef boost::shared_ptr<StandardBombModelData> ptr;

private:
	static ptr instance;

protected:
	StandardBombModelData();

public:
	static ptr getInstance();
};