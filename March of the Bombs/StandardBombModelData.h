#pragma once

#include "Model3DS.h"

#include <boost/shared_ptr.hpp>

/**
 * StandardBombModelData represents the raw data of a bomb model.
 */
class StandardBombModelData :
	public Model3DS
{
public:
	typedef boost::shared_ptr<StandardBombModelData> ptr;

private:
	static ptr instance;

protected:
	/// Constructor. Loads the model file and the texture
	StandardBombModelData();

public:
	/// Get a singleton instance, creating it first if needed
	static ptr getInstance();
};