#include "SelectionList.h"


SelectionList::SelectionList(TextureSection Background, Rectanglef PosRect, float depth)
	:Background(Background),PosRect(PosRect),depth(depth)
{
	//sets the size of the strings
	StringHightRect.setSize(glm::vec2(PosRect.getSize().x,PosRect.getSize().y * 0.1f));
	index = -1;
}


SelectionList::~SelectionList()
{
}

void SelectionList::render(Graphics::ptr graphics)
{	
	//The size of the letters
	float letterHight = StringHightRect.getSize().y;
	float letterWidth = letterHight * 0.5f;
	
	Rectanglef tempRect(glm::vec2(PosRect.getPosition().x,PosRect.getPosition().y + PosRect.getSize().y - letterHight),glm::vec2(letterWidth,letterHight));
	
	FontandKeyMapping::image_map& tempkeymap = FaKM.getKeyMap();

	for(unsigned int i = 0; i < GameList.size();i++)
	{
		for(unsigned int j = 0; j < GameList[i].size(); j++)
		{
			graphics->drawTexture(tempkeymap[GameList[i][j]],tempRect,depth-0.01f, glm::vec3(0.f));
			tempRect.setPosition(glm::vec2((tempRect.getPosition().x + letterWidth),tempRect.getPosition().y));
		}
		//skapa en rektangel runt hela strängen... 
		//som ska ge ID till vilken man tryckte på och 
		// därefter ge vilket game man ska joina... 
		
		//Rectanglef StringRect

		tempRect.setPosition(glm::vec2((PosRect.getPosition().x),tempRect.getPosition().y - letterHight));
	}

	graphics->drawTexture(Background,PosRect,depth, glm::vec3(1.f));
}

void SelectionList::updateselection(MouseButtonEvent const* mbEvent)
{
	if(PosRect.intersects(mbEvent->position) &&
		mbEvent->button == MouseButton::Left &&
		mbEvent->state == MouseButtonState::Pressed)
	{
		int tempindex = (int)(10.f - ((mbEvent->position.y - PosRect.getPosition().y) / PosRect.getSize().y) * 10);

		if(tempindex >= 0 && tempindex < (int)GameList.size())
		{
			index = tempindex;
		}
	}
}

int SelectionList::getindex()
{
	return index;
}