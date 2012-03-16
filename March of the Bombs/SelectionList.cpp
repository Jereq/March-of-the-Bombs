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
	float letterHight = StringHightRect.getSize().y;
	float letterWidth = letterHight * 0.5f;
	
	Rectanglef tempRect(glm::vec2(PosRect.getPosition().x,PosRect.getPosition().y + PosRect.getSize().y - letterHight),glm::vec2(letterWidth,letterHight));
	
	FontandKeyMapping::image_map& tempkeymap = FaKM.getKeyMap();

	for(unsigned int i = 0; i < GameList.size();i++)
	{
		for(unsigned int j = 0; j < GameList[i].size(); j++)
		{
			graphics->drawTexture(tempkeymap[GameList[i][j]],tempRect,depth);
			tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
		}

		tempRect.setPosition(glm::vec2((PosRect.getPosition().x),tempRect.getPosition().y - letterHight));
	}

	graphics->drawTexture(Background,PosRect,depth);
}