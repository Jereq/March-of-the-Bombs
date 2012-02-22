#include "GUIMain.h"
#include "MainMeny.h"

GUIMain::GUIMain()
{
	screen = Screen::ptr(new MainMeny());				//creating a smartpointer attribute, allso the thing that will say what screen to use
}

GUIMain::~GUIMain()
{
}

//the final updatefunktion that uses the updateinfo from the currently pointing screen
void GUIMain::update()
{
	//add some form of cheeck when to chance screen.. some funktion/bool or something simular
	screen->update();									//Pointing this funktion to use the pointers targets update 

	Screen::ptr next = screen->getNextScreen();
	while (next)
	{
		screen = next;
		next = screen->getNextScreen();
	}
}

//the final drawfunktion that uses the drawinfo from the currently pointing screen
void GUIMain::draw(Graphics::ptr graphics)
{
	screen->draw(graphics);								//Pointing this funktion to use the pointers targets draw
}