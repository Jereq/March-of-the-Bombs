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
	//L�gga till ngn form av koll n�r det ska bytas sk�rm... I form av funktion/bool eller om ngt annat funkar b�ttre
	screen->update();
}

void GUIMain::draw(Graphics::ptr graphics)
{
	screen->draw(graphics);
}