#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

#include "KeyboardEvent.h"

enum TFState
{
	NotInUse,
	InUse
};

class TextField
{
private:
	TextureSection Background;
	Rectanglef PosRect;
	float depth;
	bool target;
	std::string Text;
	TFState textfieldState;

	typedef std::map<char,TextureSection> image_map;
	static image_map keyMap;

public:
	typedef boost::shared_ptr<TextField> ptr;

	TextField(TextureSection Background, Rectanglef PosRect, float depth);
	~TextField();

	TFState getState();
	void setState(TFState TFS);
	void render(Graphics::ptr graphics);
	void static keyMapping();
	void updateString(KeyboardEvent* keyEvent);

	bool intersects(glm::vec2 const& point) const;
	bool istarget() const;
};

