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
	graphics->drawTexture(Background,PosRect,depth);
}