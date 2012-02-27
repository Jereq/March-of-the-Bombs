#pragma once

#include "Model.h"

class PlaneModel :
	public Model
{
public:
	typedef boost::shared_ptr<PlaneModel> ptr;

	PlaneModel();
};