#include "MainMeny.h"

MainMeny::MainMeny()
{
	MainMeny::createBackground();				//creates the vector holding all the backgrounds for this screen
	MainMeny::createButtons();					//creates the vector holding all the buttons for this screen
}

MainMeny::~MainMeny()
{
}

//Mainmenys updateinfo
void MainMeny::update()
{
	/*
	for(int i = 0; i < buttons.size(); i++)
	{
		if(mouserect.intersects(buttons[i].rectanglef)
		{
			buttons[i].changeState();
		}
	}
	*/
}

//MainMenys drawinfo
void MainMeny::draw(Graphics::ptr graphics)
{
	
	//starts to render all the buttons
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i].render(graphics);
	}	

	//starts to render all the backgrounds
	for(unsigned int i = 0; i < Backgrounds.size(); i++)
	{
		Backgrounds[i].render(graphics);
	}

}

void MainMeny::createButtons()
{
	GLTexture::ptr TestButton1 = GLTexture::loadFromFileTGA("images/skull.tga");

	//class		name	unpressed		pressed		rectangle	lower left corner	upper right corner
	Button tempbutton0(TestButton1, TestButton1, Rectanglef(glm::vec2(0.25f,0.42f),glm::vec2(0.50f,0.15f)));
	Button tempbutton1(TestButton1, TestButton1, Rectanglef(glm::vec2(0.30f,0.32f),glm::vec2(0.40f,0.08f)));
	Button tempbutton2(TestButton1, TestButton1, Rectanglef(glm::vec2(0.33f,0.22f),glm::vec2(0.35f,0.08f)));
	Button tempbutton3(TestButton1, TestButton1, Rectanglef(glm::vec2(0.36f,0.12f),glm::vec2(0.30f,0.08f)));
	Button tempbutton4(TestButton1, TestButton1, Rectanglef(glm::vec2(0.39f,0.02f),glm::vec2(0.25f,0.06f)));


	buttons.push_back(tempbutton0);
	buttons.push_back(tempbutton1);
	buttons.push_back(tempbutton2);
	buttons.push_back(tempbutton3);
	buttons.push_back(tempbutton4);
}

void MainMeny::createBackground()
{
	GLTexture::ptr TestBackground1 = GLTexture::loadFromFileTGA("images/skull.tga");

	SimpleImage Background1(TestBackground1, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)));

	Backgrounds.push_back(Background1);
}