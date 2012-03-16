#pragma once
#include "TextureSection.h"
#include "Rectanglef.h"
#include <glm\glm.hpp>

class FontandKeyMapping
{
public:

	typedef std::map<char,TextureSection> image_map;
	typedef boost::shared_ptr<FontandKeyMapping> ptr;

private:
		
	static image_map keyMap;

public:	

	FontandKeyMapping();
	~FontandKeyMapping();

	void static keyMapping();
	image_map& getKeyMap();
};

