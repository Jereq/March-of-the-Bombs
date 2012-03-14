#include "SelectionList.h"


SelectionList::SelectionList(TextureSection Background, Rectanglef PosRect, float depth)
	:Background(Background),PosRect(PosRect),depth(depth)
{
}


SelectionList::~SelectionList()
{
}

void SelectionList::render(Graphics::ptr graphics)
{
	//ngn form av loop som gör detta för varje sträng som skickats in...
	/*
	for(unsigned int i = 0; i < somethingsomthing.size();i++)
	{
		Rectanglef tempRect = PosRect;
		float letterWidth = PosRect.getSize().y * 0.5f;
		tempRect.setSize(glm::vec2(letterWidth,PosRect.getSize().y));

		for(unsigned int i = 0; i < Text.length(); i++)
		{
			graphics->drawTexture(keyMap[Text[i]],tempRect,depth);
			tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
		}
	}
	*/
	graphics->drawTexture(Background,PosRect,depth);
}