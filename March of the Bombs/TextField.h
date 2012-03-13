#pragma once

#include "TextureSection.h"
#include "Rectanglef.h"
#include "Graphics.h"
#include <glm\glm.hpp>

#include "KeyboardEvent.h"

enum TFState
{
	NotTargeted,
	Targeted
};

class TextField
{
private:
	TextureSection deactiveBackground, activeBackground;
	Rectanglef PosRect;
	float depth;
	bool targeted;
	std::string Text;
	TFState textfieldState;

	typedef std::map<char,TextureSection> image_map;
	static image_map keyMap;

public:
	typedef boost::shared_ptr<TextField> ptr;

	bool active;

	TextField(TextureSection activeBackground, TextureSection deactiveBackground, Rectanglef PosRect, float depth);
	~TextField();

	TFState getState();
	void setState(TFState TFS);
	void render(Graphics::ptr graphics);
	void static keyMapping();
	void updateString(KeyboardEvent* keyEvent);

	string getString();

	bool intersects(glm::vec2 const& point) const;
	bool istargeted() const;
};