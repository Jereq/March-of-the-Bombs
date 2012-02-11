#include "GUIMain.h"
#include "MainMeny.h"

GUIMain::GUIMain()
{
	screen = Screen::ptr(new MainMeny());
}

GUIMain::~GUIMain()
{
}

void GUIMain::update()
{
	//Lägga till ngn form av koll när det ska bytas skärm... I form av funktion/bool eller om ngt annat funkar bättre
	screen->update();
}

void GUIMain::draw(Graphics::ptr graphics)
{
	screen->draw(graphics);
}