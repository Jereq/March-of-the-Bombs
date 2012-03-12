#include "TextField.h"

TextField::image_map TextField::keyMap;


TextField::TextField(TextureSection Background, Rectanglef PosRect, float depth)
	: Background(Background), PosRect(PosRect), depth(depth)
{
	keyMapping();
	bool target = false;
}


TextField::~TextField()
{
}

void TextField::render(Graphics::ptr graphics)
{
	Rectanglef tempRect = PosRect;
	float letterWidth = PosRect.getSize().y * 0.5f;
	tempRect.setSize(glm::vec2(letterWidth,PosRect.getSize().y));

	for(unsigned int i = 0; i < Text.length(); i++)
	{
		graphics->drawTexture(keyMap[Text[i]],tempRect,depth);
		tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
	}

	graphics->drawTexture(Background,PosRect,0.50f);
}

void TextField::keyMapping()
{
	if(!keyMap.empty())
	{
		return;
	}
	else
	{
		TextureSection BitFont(L"Images/BitFont.png");

		float sizef = 1 / 8.f;
		glm::vec2 lettersize(sizef);

		//small letter
		//Big letter
		keyMap['a'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 7 * sizef), lettersize));
		keyMap['A'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 7 * sizef), lettersize));

		keyMap['b'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 7 * sizef), lettersize));
		keyMap['B'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 7 * sizef), lettersize));

		keyMap['c'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 7 * sizef), lettersize));
		keyMap['C'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 7 * sizef), lettersize));

		keyMap['d'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 7 * sizef), lettersize));
		keyMap['D'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 7 * sizef), lettersize));

		keyMap['e'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 7 * sizef), lettersize));
		keyMap['E'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 7 * sizef), lettersize));

		keyMap['f'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 7 * sizef), lettersize));
		keyMap['F'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 7 * sizef), lettersize));

		keyMap['g'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 7 * sizef), lettersize));
		keyMap['G'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 7 * sizef), lettersize));

		keyMap['h'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 7 * sizef), lettersize));
		keyMap['H'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 7 * sizef), lettersize));

		keyMap['i'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 6 * sizef), lettersize));
		keyMap['I'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 6 * sizef), lettersize));

		keyMap['j'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 6 * sizef), lettersize));
		keyMap['J'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 6 * sizef), lettersize));

		keyMap['k'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 6 * sizef), lettersize));
		keyMap['K'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 6 * sizef), lettersize));

		keyMap['l'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 6 * sizef), lettersize));
		keyMap['L'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 6 * sizef), lettersize));

		keyMap['m'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 6 * sizef), lettersize));
		keyMap['M'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 6 * sizef), lettersize));

		keyMap['n'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 6 * sizef), lettersize));
		keyMap['N'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 6 * sizef), lettersize));

		keyMap['o'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 6 * sizef), lettersize));
		keyMap['O'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 6 * sizef), lettersize));

		keyMap['p'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 6 * sizef), lettersize));
		keyMap['P'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 6 * sizef), lettersize));

		keyMap['q'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 5 * sizef), lettersize));
		keyMap['Q'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 5 * sizef), lettersize));

		keyMap['r'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 5 * sizef), lettersize));
		keyMap['R'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 5 * sizef), lettersize));

		keyMap['s'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 5 * sizef), lettersize));
		keyMap['S'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 5 * sizef), lettersize));

		keyMap['t'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 5 * sizef), lettersize));
		keyMap['T'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 5 * sizef), lettersize));

		keyMap['u'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 5 * sizef), lettersize));
		keyMap['U'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 5 * sizef), lettersize));

		keyMap['v'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 5 * sizef), lettersize));
		keyMap['V'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 5 * sizef), lettersize));

		keyMap['w'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 5 * sizef), lettersize));
		keyMap['W'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 5 * sizef), lettersize));

		keyMap['x'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 5 * sizef), lettersize));
		keyMap['X'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 5 * sizef), lettersize));

		keyMap['y'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 4 * sizef), lettersize));
		keyMap['Y'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 4 * sizef), lettersize));

		keyMap['z'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 4 * sizef), lettersize));
		keyMap['Z'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 4 * sizef), lettersize));

		//Numbers
		keyMap['0'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 4 * sizef), lettersize));

		keyMap['1'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 4 * sizef), lettersize));

		keyMap['2'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 4 * sizef), lettersize));

		keyMap['3'] = BitFont.cut(Rectanglef(glm::vec2(5 * sizef, 4 * sizef), lettersize));

		keyMap['4'] = BitFont.cut(Rectanglef(glm::vec2(6 * sizef, 4 * sizef), lettersize));

		keyMap['5'] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 4 * sizef), lettersize));

		keyMap['6'] = BitFont.cut(Rectanglef(glm::vec2(0 * sizef, 3 * sizef), lettersize));

		keyMap['7'] = BitFont.cut(Rectanglef(glm::vec2(1 * sizef, 3 * sizef), lettersize));

		keyMap['8'] = BitFont.cut(Rectanglef(glm::vec2(2 * sizef, 3 * sizef), lettersize));

		keyMap['9'] = BitFont.cut(Rectanglef(glm::vec2(3 * sizef, 3 * sizef), lettersize));

		//special tokens
		keyMap['.'] = BitFont.cut(Rectanglef(glm::vec2(4 * sizef, 3 * sizef), lettersize));

		keyMap[' '] = BitFont.cut(Rectanglef(glm::vec2(7 * sizef, 0 * sizef), lettersize));
	}
}

void TextField::updateString(KeyboardEvent* keyEvent)
{
	char eventKey = keyEvent->key;

	if (keyMap.count(eventKey) == 1)
	{
		Text += eventKey;
	}
	//else
	//{
	//Johnyproblem!
	//}
}

void TextField::setState(TFState TFS)
{
	textfieldState = TFS;
}

TFState TextField::getState()
{
	return textfieldState;
}

bool TextField::intersects(glm::vec2 const& point) const
{
	return PosRect.intersects(point);
}

bool TextField::istarget() const
{
	return target;
}