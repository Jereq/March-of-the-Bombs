#include "SelectionList.h"


SelectionList::SelectionList(TextureSection Background, Rectanglef PosRect, float depth)
	:Background(Background),PosRect(PosRect),depth(depth)
{
	//sets the size of the strings
	StringHightRect.setSize(glm::vec2(PosRect.getSize().x,PosRect.getSize().y * 0.1f));
}


SelectionList::~SelectionList()
{
}

void SelectionList::render(Graphics::ptr graphics)
{
	//ngn form av loop som gör detta för varje sträng som skickats in...
	
	//The size of the letters
	Rectanglef tempRect = PosRect;
	float letterWidth = PosRect.getSize().y * 0.5f;
	tempRect.setSize(glm::vec2(letterWidth,PosRect.getSize().y));

	for(unsigned int i = 0; i < GameList.size();i++)
	{
		for(unsigned int j = 0; j < GameList[i].size(); j++)
		{
			//graphics->drawTexture(GameList[i],tempRect,depth);
			tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
		}

		/*
		mall för att se hur jag gjorde med textfälten
		Rectanglef tempRect = PosRect;
		float letterWidth = PosRect.getSize().y * 0.5f;
		tempRect.setSize(glm::vec2(letterWidth,PosRect.getSize().y));

		for(unsigned int i = 0; i < Text.length(); i++)
		{
			graphics->drawTexture(keyMap[Text[i]],tempRect,depth);
			tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
		}
		*/
	}

	graphics->drawTexture(Background,PosRect,depth);
}